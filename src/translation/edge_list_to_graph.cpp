#include "translation/edge_list_to_graph.h"

AdjListGraph EdgeListToGraph(const io::EdgeListProblem& problem) {
    AdjListGraph g(problem.num_vertices, problem.directed);
    for (const auto& edge : problem.edges) {
        g.AddEdge(edge.u, edge.v, edge.w);
    }
    return g;
}
