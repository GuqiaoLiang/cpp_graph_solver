#include "io/json_parser.h"

#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "io/grid_parser.h"

namespace io {

template <typename Json>
static void AppendJsonInline(std::ostream& out, const Json& value);

template <typename Json>
static bool IsPrimitive(const Json& value) {
    return value.is_string() || value.is_boolean() || value.is_number() ||
           value.is_null();
}

template <typename Json>
static void AppendJsonPretty(std::ostream& out, const Json& value,
                             int indent, int level) {
    if (value.is_object()) {
        out << "{\n";
        int index = 0;
        for (const auto& item : value.items()) {
            out << std::string((level + 1) * indent, ' ');
            out << nlohmann::json(item.key()).dump() << ": ";
            if (item.value().is_object() || item.value().is_array()) {
                AppendJsonPretty(out, item.value(), indent, level + 1);
            } else {
                out << item.value().dump();
            }
            if (++index < static_cast<int>(value.size())) {
                out << ",";
            }
            out << "\n";
        }
        out << std::string(level * indent, ' ') << "}";
        return;
    }

    if (value.is_array()) {
        bool inline_array = true;
        for (const auto& item : value) {
            if (!IsPrimitive(item)) {
                inline_array = false;
                break;
            }
        }
        if (inline_array) {
            AppendJsonInline(out, value);
            return;
        }
        out << "[\n";
        for (size_t i = 0; i < value.size(); ++i) {
            out << std::string((level + 1) * indent, ' ');
            AppendJsonPretty(out, value[i], indent, level + 1);
            if (i + 1 < value.size()) {
                out << ",";
            }
            out << "\n";
        }
        out << std::string(level * indent, ' ') << "]";
        return;
    }

    out << value.dump();
}

template <typename Json>
static void AppendJsonInline(std::ostream& out, const Json& value) {
    if (value.is_array()) {
        out << "[";
        for (size_t i = 0; i < value.size(); ++i) {
            if (i > 0) {
                out << ", ";
            }
            AppendJsonInline(out, value[i]);
        }
        out << "]";
        return;
    }

    if (value.is_object()) {
        out << "{";
        int index = 0;
        for (const auto& item : value.items()) {
            if (index++ > 0) {
                out << ", ";
            }
            out << nlohmann::json(item.key()).dump() << ": ";
            AppendJsonInline(out, item.value());
        }
        out << "}";
        return;
    }

    out << value.dump();
}

static Edge ParseEdge(const nlohmann::json& edge) {
    if (!edge.is_array() || (edge.size() != 2 && edge.size() != 3)) {
        throw std::runtime_error("Edge must be [u,v] or [u,v,w]");
    }
    Edge e;
    e.u = edge.at(0).get<int>();
    e.v = edge.at(1).get<int>();
    if (edge.size() == 3) {
        e.w = edge.at(2).get<int>();
    }
    return e;
}

static EdgeListProblem ParseEdgeListProblem(const nlohmann::json& problem) {
    EdgeListProblem out;
    out.num_vertices = problem.at("num_vertices").get<int>();
    out.directed = problem.value("directed", false);
    for (const auto& edge : problem.at("edges")) {
        out.edges.push_back(ParseEdge(edge));
    }
    return out;
}

static FlowProblem ParseFlowProblem(const nlohmann::json& problem) {
    FlowProblem out;
    out.num_nodes = problem.at("num_nodes").get<int>();
    out.source = problem.at("source").get<int>();
    out.sink = problem.at("sink").get<int>();
    for (const auto& edge : problem.at("edges")) {
        out.edges.push_back(ParseEdge(edge));
    }
    return out;
}

static NQueensProblem ParseNQueensProblem(const nlohmann::json& problem) {
    NQueensProblem out;
    out.n = problem.at("n").get<int>();
    return out;
}

static SearchProblem ParseSearchProblem(const nlohmann::json& problem) {
    SearchProblem out;
    out.type = problem.at("type").get<std::string>();
    if (out.type == "grid") {
        out.problem = ParseGridProblem(problem);
    } else if (out.type == "n_queens") {
        out.problem = ParseNQueensProblem(problem);
    } else {
        throw std::runtime_error("Unsupported search problem type: " + out.type);
    }
    return out;
}

static SearchProblem ParseLegacyGridSearchProblem(const nlohmann::json& problem) {
    SearchProblem out;
    out.type = "grid";
    out.problem = ParseGridProblem(problem);
    return out;
}

static InputSpec ParseJsonRoot(const nlohmann::json& root) {
    InputSpec spec;
    spec.mode = root.at("mode").get<std::string>();

    const auto& problem = root.at("problem");
    if (spec.mode == "edge_list") {
        spec.problem = ParseEdgeListProblem(problem);
    } else if (spec.mode == "grid") {
        spec.problem = ParseLegacyGridSearchProblem(problem);
    } else if (spec.mode == "flow") {
        spec.problem = ParseFlowProblem(problem);
    } else if (spec.mode == "search") {
        spec.problem = ParseSearchProblem(problem);
    } else {
        throw std::runtime_error("Unsupported mode: " + spec.mode);
    }

    for (const auto& query : root.at("queries")) {
        Query q;
        q.raw = query;
        q.type = query.at("type").get<std::string>();
        spec.queries.push_back(std::move(q));
    }

    return spec;
}

InputSpec ParseJsonText(const std::string& text) {
    nlohmann::json root = nlohmann::json::parse(text);
    return ParseJsonRoot(root);
}

InputSpec ParseJsonFile(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Failed to open input file: " + path);
    }
    nlohmann::json root;
    input >> root;
    return ParseJsonRoot(root);
}

void WriteJsonFile(const nlohmann::json& output, const std::string& path) {
    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Failed to open output file: " + path);
    }
    AppendJsonPretty(out, output, 2, 0);
}

void WriteJsonFile(const nlohmann::ordered_json& output, const std::string& path) {
    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Failed to open output file: " + path);
    }
    AppendJsonPretty(out, output, 2, 0);
}

}  // namespace io
