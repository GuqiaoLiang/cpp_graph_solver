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
#include "translation/edge_list_to_graph.h"
#include "translation/grid_to_graph.h"
#include "translation/task_to_flow.h"

using nlohmann::json;

namespace {

json DistancesToJson(const std::vector<int>& dist) {
    json out = json::array();
    for (int d : dist) {
        if (d < 0) {
            out.push_back(nullptr);
        } else {
            out.push_back(d);
        }
    }
    return out;
}

json DistancesToJson(const std::vector<long long>& dist) {
    const long long INF = 1e18 / 4;
    json out = json::array();
    for (long long d : dist) {
        if (d >= INF) {
            out.push_back(nullptr);
        } else {
            out.push_back(d);
        }
    }
    return out;
}

json PathToJson(const std::vector<int>& path) {
    json out = json::array();
    for (int v : path) {
        out.push_back(v);
    }
    return out;
}

json GridPathToJson(const GridGraph& grid, const std::vector<int>& path) {
    json out = json::array();
    for (int v : path) {
        auto cell = GridVertexToCell(grid, v);
        out.push_back({cell[0], cell[1]});
    }
    return out;
}

json RunEdgeListQuery(const AdjListGraph& graph, const io::EdgeListProblem& problem,
                      const io::Query& query) {
    if (query.type == "connected_components") {
        auto res = Connected_Component(graph);
        return json{{"count", res.components}, {"component_ids", res.cids}};
    }

    if (query.type == "contains_cycle") {
        if (graph.IsDirected()) {
            return json{{"has_cycle", !IsDAG(graph)}};
        }
        return json{{"has_cycle", HasCycleUndirected(graph)}};
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

        json edges = json::array();
        for (const auto& [u, v, w] : res.edges) {
            edges.push_back({u, v, w});
        }
        return json{{"total_weight", res.total_weight}, {"edges", edges}};
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
            json out{{"distances", DistancesToJson(res.dist)}};
            if (has_target) {
                auto path = ReconstructPath(res.parent, source, target);
                out["path_to_target"] = PathToJson(path);
            }
            return out;
        }
        if (method == "dijkstra") {
            auto res = DijkstraShortestPath(graph, source);
            json out{{"distances", DistancesToJson(res.dist)}};
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

json RunGridQuery(const GridGraph& grid, const io::GridProblem& problem,
                  const io::Query& query) {
    if (query.type != "shortest_path") {
        throw std::runtime_error("Unsupported grid query type: " + query.type);
    }

    std::string method = query.raw.value("method", "bfs");
    int start = GridCellToVertex(grid, problem.start[0], problem.start[1]);
    int target = GridCellToVertex(grid, problem.target[0], problem.target[1]);

    if (start < 0 || target < 0) {
        throw std::runtime_error("Grid start/target not walkable");
    }

    if (method == "bfs") {
        auto res = GraphBFSWithParent(grid.graph, start);
        auto path = ReconstructPath(res.parent, start, target);
        json out;
        if (res.dist[target] < 0) {
            out["distance"] = nullptr;
        } else {
            out["distance"] = res.dist[target];
        }
        out["path_to_target"] = GridPathToJson(grid, path);
        return out;
    }

    if (method == "astar") {
        auto heuristic = [&](int u, int t) {
            auto cu = GridVertexToCell(grid, u);
            auto ct = GridVertexToCell(grid, t);
            return static_cast<long long>(
                std::abs(cu[0] - ct[0]) + std::abs(cu[1] - ct[1]));
        };
        auto res = AStarSearch(grid.graph, start, target, heuristic);
        auto path = ReconstructPath(res.parent, start, target);
        json out;
        if (res.dist[target] >= 1e18 / 4) {
            out["distance"] = nullptr;
        } else {
            out["distance"] = res.dist[target];
        }
        out["path_to_target"] = GridPathToJson(grid, path);
        return out;
    }

    throw std::runtime_error("Unsupported grid shortest_path method: " + method);
}

json RunFlowQuery(const io::FlowProblem& problem, const io::Query& query) {
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
    return json{{"max_flow", flow}};
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

        json output;
        output["mode"] = spec.mode;
        output["results"] = json::array();

        if (spec.mode == "edge_list") {
            const auto& problem = std::get<io::EdgeListProblem>(spec.problem);
            AdjListGraph graph = EdgeListToGraph(problem);
            for (const auto& query : spec.queries) {
                output["results"].push_back({
                    {"query", query.raw},
                    {"result", RunEdgeListQuery(graph, problem, query)}
                });
            }
        } else if (spec.mode == "grid") {
            const auto& problem = std::get<io::GridProblem>(spec.problem);
            GridGraph grid = GridToGraph(problem);
            for (const auto& query : spec.queries) {
                output["results"].push_back({
                    {"query", query.raw},
                    {"result", RunGridQuery(grid, problem, query)}
                });
            }
        } else if (spec.mode == "flow") {
            const auto& problem = std::get<io::FlowProblem>(spec.problem);
            for (const auto& query : spec.queries) {
                output["results"].push_back({
                    {"query", query.raw},
                    {"result", RunFlowQuery(problem, query)}
                });
            }
        } else {
            throw std::runtime_error("Unsupported mode: " + spec.mode);
        }

        auto finished = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = finished - started;
        output["stats"] = {{"time_ms", elapsed.count()}};
        output["meta"] = {{"solver_version", "v1.0"}};

        io::WriteJsonFile(output, output_path);
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
