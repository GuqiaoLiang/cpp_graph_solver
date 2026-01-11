#include "algorithms/directed/topo_sort.h"
#include <vector>
#include <algorithm>

std::vector<int> TopologicalSort(const Graph& g) {

    std::vector<bool> visited(g.NumVertices(), false);

    std::vector<int> res;

    for (int i = 0; i < g.NumVertices(); i ++) {
        if (! visited[i]) {
            dfs_topo_Helper(g, i, visited, res);
        }
    }

    std::reverse(res.begin(), res.end());

    return res;

}

void dfs_topo_Helper(const Graph& g, int s, std::vector<bool>& visited, std::vector<int>& res) {
    visited[s] = true;

    for (const auto& [u, w] : g.Neighbors(s)) {
        if (visited[u]) {
            continue;
        }

        dfs_topo_Helper(g, u, visited, res);
    }

    res.push_back(s);

    return;


}
