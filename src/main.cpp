#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "algorithms/connectivity/connected_components.h"
#include "algorithms/connectivity/cycle_detection.h"
#include "algorithms/dag/is_dag.h"
#include "algorithms/flow/max_flow.h"
#include "algorithms/mst/mst.h"
#include "algorithms/shortest_path/astar.h"
#include "algorithms/shortest_path/bfs.h"
#include "algorithms/shortest_path/dijkstra.h"
#include "io/input_parser.h"
#include "io/json_parser.h"
#include "search/astar.h"
#include "search/bfs.h"
#include "search/dfs.h"
#include "search/problems/grid_problem.h"
#include "search/problems/nqueens_problem.h"
#include "translation/edge_list_to_graph.h"

using nlohmann::json;
using nlohmann::ordered_json;

namespace {

ordered_json DistancesToJson(const std::vector<int>& dist) {
    ordered_json out = ordered_json::array();
    for (int d : dist) {
        if (d < 0) {
            out.push_back(nullptr);
        } else {
            out.push_back(d);
        }
    }
    return out;
}

ordered_json DistancesToJson(const std::vector<long long>& dist) {
    const long long INF = 1e18 / 4;
    ordered_json out = ordered_json::array();
    for (long long d : dist) {
        if (d >= INF) {
            out.push_back(nullptr);
        } else {
            out.push_back(d);
        }
    }
    return out;
}

ordered_json PathToJson(const std::vector<int>& path) {
    ordered_json out = ordered_json::array();
    for (int v : path) {
        out.push_back(v);
    }
    return out;
}

ordered_json GridPathToJson(const std::vector<search::GridCell>& path) {
    ordered_json out = ordered_json::array();
    for (const auto& cell : path) {
        out.push_back({cell.row, cell.col});
    }
    return out;
}

ordered_json NQueensSolutionToJson(const std::vector<int>& cols) {
    ordered_json out = ordered_json::array();
    for (int col : cols) {
        out.push_back(col);
    }
    return out;
}

ordered_json QueryToJson(const io::Query& query) {
    ordered_json out;
    out["type"] = query.type;
    for (const auto& item : query.raw.items()) {
        if (item.key() == "type") {
            continue;
        }
        out[item.key()] = item.value();
    }
    return out;
}

ordered_json RunEdgeListQuery(const AdjListGraph& graph,
                              const io::EdgeListProblem& problem,
                              const io::Query& query) {
    if (query.type == "connected_components") {
        auto res = Connected_Component(graph);
        ordered_json out;
        out["count"] = res.components;
        out["component_ids"] = res.cids;
        return out;
    }

    if (query.type == "contains_cycle") {
        if (graph.IsDirected()) {
            ordered_json out;
            out["has_cycle"] = !IsDAG(graph);
            return out;
        }
        ordered_json out;
        out["has_cycle"] = HasCycleUndirected(graph);
        return out;
    }

    if (query.type == "mst") {
        std::string method = query.raw.value("method", "kruskal");
        MSTResult res;
        if (method == "prim") {
            int start = query.raw.value("start", 0);
            res = PrimMST(graph, start);
        } else if (method == "kruskal") {
            res = KruskalMST(graph);
        } else {
            throw std::runtime_error("Unsupported MST method: " + method);
        }

        ordered_json edges = ordered_json::array();
        for (const auto& [u, v, w] : res.edges) {
            edges.push_back({u, v, w});
        }
        ordered_json out;
        out["total_weight"] = res.total_weight;
        out["edges"] = edges;
        return out;
    }

    if (query.type == "shortest_path") {
        int source = query.raw.at("source").get<int>();
        bool weighted = false;
        for (const auto& edge : problem.edges) {
            if (edge.w != 1) {
                weighted = true;
                break;
            }
        }
        std::string method = query.raw.value("method", weighted ? "dijkstra" : "bfs");
        bool has_target = query.raw.contains("target");
        int target = has_target ? query.raw.at("target").get<int>() : -1;

        if (method == "bfs") {
            auto res = GraphBFSWithParent(graph, source);
            ordered_json out;
            out["distances"] = DistancesToJson(res.dist);
            if (has_target) {
                auto path = ReconstructPath(res.parent, source, target);
                out["path_to_target"] = PathToJson(path);
            }
            return out;
        }
        if (method == "dijkstra") {
            auto res = DijkstraShortestPath(graph, source);
            ordered_json out;
            out["distances"] = DistancesToJson(res.dist);
            if (has_target) {
                auto path = ReconstructPath(res.parent, source, target);
                out["path_to_target"] = PathToJson(path);
            }
            return out;
        }
        throw std::runtime_error("Unsupported shortest_path method: " + method);
    }

    throw std::runtime_error("Unsupported query type: " + query.type);
}

ordered_json RunGridSearchQuery(const io::GridProblem& problem,
                                const io::Query& query) {
    if (query.type != "shortest_path") {
        throw std::runtime_error("Unsupported grid query type: " + query.type);
    }

    std::string method = query.raw.value("method", "bfs");
    search::GridSearchProblem grid(problem);
    if (!grid.IsWalkable(grid.Start()) || !grid.IsWalkable(grid.Target())) {
        throw std::runtime_error("Grid start/target not walkable");
    }

    search::SearchResult<search::GridCell> res;
    if (method == "bfs") {
        res = search::BFS(grid, grid.Start());
    } else if (method == "dfs") {
        res = search::DFSFindOne(grid, grid.Start());
    } else if (method == "astar") {
        auto target = grid.Target();
        std::function<long long(const search::GridCell&)> heuristic =
            [&](const search::GridCell& cell) -> long long {
            int dr = std::abs(cell.row - target.row);
            int dc = std::abs(cell.col - target.col);
            if (problem.allow_diagonal) {
                return std::max(dr, dc);
            }
            return dr + dc;
        };
        res = search::AStarSearch(grid, grid.Start(), heuristic);
    } else {
        throw std::runtime_error("Unsupported grid shortest_path method: " + method);
    }

    ordered_json out;
    if (!res.found) {
        out["distance"] = nullptr;
        out["path_to_target"] = ordered_json::array();
    } else {
        out["distance"] = static_cast<int>(res.path.size()) - 1;
        out["path_to_target"] = GridPathToJson(res.path);
    }
    return out;
}

ordered_json RunNQueensQuery(const io::NQueensProblem& problem,
                             const io::Query& query) {
    search::NQueensProblem nqueens(problem.n);
    if (query.type == "find_one") {
        auto res = search::DFSFindOne(nqueens, nqueens.Start());
        ordered_json out;
        out["solutions"] = ordered_json::array();
        if (res.found) {
            out["solutions"].push_back(NQueensSolutionToJson(res.path.back().cols));
        }
        return out;
    }
    if (query.type == "count_solutions") {
        auto res = search::DFSCountSolutions(nqueens, nqueens.Start());
        ordered_json out;
        out["count"] = res.count;
        if (!res.example_path.empty()) {
            out["solutions"] = ordered_json::array();
            out["solutions"].push_back(
                NQueensSolutionToJson(res.example_path.back().cols));
        }
        return out;
    }
    throw std::runtime_error("Unsupported n_queens query type: " + query.type);
}

ordered_json RunSearchQuery(const io::SearchProblem& problem,
                            const io::Query& query) {
    if (problem.type == "grid") {
        const auto& grid = std::get<io::GridProblem>(problem.problem);
        return RunGridSearchQuery(grid, query);
    }
    if (problem.type == "n_queens") {
        const auto& nq = std::get<io::NQueensProblem>(problem.problem);
        return RunNQueensQuery(nq, query);
    }
    throw std::runtime_error("Unsupported search problem type: " + problem.type);
}

ordered_json RunFlowQuery(const io::FlowProblem& problem, const io::Query& query) {
    if (query.type != "max_flow") {
        throw std::runtime_error("Unsupported flow query type: " + query.type);
    }
    std::string method = query.raw.value("method", "dinic");
    if (method != "dinic") {
        throw std::runtime_error("Unsupported max_flow method: " + method);
    }

    Dinic dinic(problem.num_nodes);
    for (const auto& edge : problem.edges) {
        dinic.AddEdge(edge.u, edge.v, edge.w);
    }
    int flow = dinic.MaxFlow(problem.source, problem.sink);
    ordered_json out;
    out["max_flow"] = flow;
    return out;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: graph_solver input.json output.json\n";
        return 1;
    }

    const std::string input_path = argv[1];
    const std::string output_path = argv[2];

    try {
        auto started = std::chrono::steady_clock::now();

        io::InputSpec spec = io::ParseJsonFile(input_path);

        ordered_json output;
        output["mode"] = spec.mode;
        output["results"] = ordered_json::array();

        if (spec.mode == "edge_list") {
            const auto& problem = std::get<io::EdgeListProblem>(spec.problem);
            AdjListGraph graph = EdgeListToGraph(problem);
            for (const auto& query : spec.queries) {
                ordered_json entry;
                entry["query"] = QueryToJson(query);
                entry["result"] = RunEdgeListQuery(graph, problem, query);
                output["results"].push_back(entry);
            }
        } else if (spec.mode == "grid") {
            const auto& search_problem = std::get<io::SearchProblem>(spec.problem);
            for (const auto& query : spec.queries) {
                ordered_json entry;
                entry["query"] = QueryToJson(query);
                entry["result"] = RunSearchQuery(search_problem, query);
                output["results"].push_back(entry);
            }
        } else if (spec.mode == "flow") {
            const auto& problem = std::get<io::FlowProblem>(spec.problem);
            for (const auto& query : spec.queries) {
                ordered_json entry;
                entry["query"] = QueryToJson(query);
                entry["result"] = RunFlowQuery(problem, query);
                output["results"].push_back(entry);
            }
        } else if (spec.mode == "search") {
            const auto& problem = std::get<io::SearchProblem>(spec.problem);
            for (const auto& query : spec.queries) {
                ordered_json entry;
                entry["query"] = QueryToJson(query);
                entry["result"] = RunSearchQuery(problem, query);
                output["results"].push_back(entry);
            }
        } else {
            throw std::runtime_error("Unsupported mode: " + spec.mode);
        }

        auto finished = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = finished - started;
        ordered_json stats;
        stats["time_ms"] = elapsed.count();
        output["stats"] = stats;
        ordered_json meta;
        meta["solver_version"] = "v1.0";
        output["meta"] = meta;

        io::WriteJsonFile(output, output_path);
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
