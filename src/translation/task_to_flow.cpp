#include "translation/task_to_flow.h"

AdjListGraph TaskToFlowGraph(const io::FlowProblem& problem) {
    AdjListGraph g(problem.num_nodes, true);
    for (const auto& edge : problem.edges) {
        g.AddEdge(edge.u, edge.v, edge.w);
    }
    return g;
}
