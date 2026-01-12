#pragma once
#include <array>
#include <string>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>

namespace io {

struct Edge {
    int u = 0;
    int v = 0;
    int w = 1;
};

struct EdgeListProblem {
    int num_vertices = 0;
    bool directed = false;
    std::vector<Edge> edges;
};

struct GridProblem {
    int rows = 0;
    int cols = 0;
    std::vector<std::string> map;
    std::array<int, 2> start{0, 0};
    std::array<int, 2> target{0, 0};
    bool allow_diagonal = false;
};

struct FlowProblem {
    int num_nodes = 0;
    int source = 0;
    int sink = 0;
    std::vector<Edge> edges;
};

struct NQueensProblem {
    int n = 0;
};

struct SearchProblem {
    std::string type;
    std::variant<GridProblem, NQueensProblem> problem;
};

struct Query {
    std::string type;
    nlohmann::json raw;
};

using Problem = std::variant<EdgeListProblem, GridProblem, FlowProblem, SearchProblem>;

struct InputSpec {
    std::string mode;
    Problem problem;
    std::vector<Query> queries;
};

InputSpec ParseJsonText(const std::string& text);
InputSpec ParseJsonFile(const std::string& path);
void WriteJsonFile(const nlohmann::json& output, const std::string& path);
void WriteJsonFile(const nlohmann::ordered_json& output, const std::string& path);

}  // namespace io
