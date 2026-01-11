#include "algorithms/directed/is_dag.h"

bool IsDAG(const Graph& g) {
    if (! g.IsDirected()) {
        return false;  // early return
    }


    int n = g.NumVertices();
    std::vector<int> color(n, 0);

    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) {
            if (dfs_contains_cycle(g, i, color))
                return false;
        }
    }
    return true;
}






bool dfs_contains_cycle(const Graph& g, int s, std::vector<int>& color) {

    color[s] = 1; // visiting

    for (const auto& [v, w] : g.Neighbors(s)) {
        if (color[v] == 1) return true;               // back edge
        if (color[v] == 0 && dfs_contains_cycle(g, v, color))  // v haven't been visited
            return true;
    }

    color[s] = 2; // done, already visited
    return false;


}
