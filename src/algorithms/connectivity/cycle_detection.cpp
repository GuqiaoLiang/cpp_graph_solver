#include "algorithms/connectivity/cycle_detection.h"
#include <stdexcept>

static bool dfs_cycle_undirected(const Graph& g, int u, int parent,
                                 std::vector<bool>& visited);


bool HasCycleUndirected(const Graph& g) {
    if (g.IsDirected()) {
        throw std::logic_error("Undirected cycle check on directed graph");
    }
    std::vector<bool> visited(g.NumVertices(), false);

    for (int i = 0; i < g.NumVertices(); i++) {
        if (!visited[i]) {
            if (dfs_cycle_undirected(g, i, -1, visited)) {
                return true;
            }
        }
    }

    return false;
}



bool dfs_cycle_undirected(const Graph& g, int u, int parent, std::vector<bool>& visited) {
    visited[u] = true;

    for (const auto& [v, w] : g.Neighbors(u)) {
        if (!visited[v]) {
            if (dfs_cycle_undirected(g, v, u, visited)) {
                return true;
            }
        }
        // v is visited but not parent
        else if (parent != v) {
            return true;
        }


    }

    return false;
}
