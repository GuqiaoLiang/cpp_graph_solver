#include "utils/reverse_graph.h"


AdjListGraph ReverseGraph(const Graph& g) {
    AdjListGraph ng = AdjListGraph(g.NumVertices(), true);

    for (int u = 0; u < g.NumVertices(); ++u) {
        for (const auto& [v, w] : g.Neighbors(u)) {
            ng.AddEdge(v, u, w);
        }
    }

    return ng;


}