#include <cassert>
#include <iostream>

#include "include/algorithms/flow/max_flow.h"
#include "include/graph/adj_list_graph.h"

namespace {
void ExpectEq(int actual, int expected, const char* test_name) {
    if (actual != expected) {
        std::cerr << "[FAIL] " << test_name << ": expected " << expected
                  << ", got " << actual << std::endl;
        std::exit(1);
    }
    std::cout << "[PASS] " << test_name << std::endl;
}
}

int main() {
    {
        // CLRS-style example graph, max flow should be 23.
        AdjListGraph g(6, true);
        g.AddEdge(0, 1, 16);
        g.AddEdge(0, 2, 13);
        g.AddEdge(1, 2, 10);
        g.AddEdge(2, 1, 4);
        g.AddEdge(1, 3, 12);
        g.AddEdge(3, 2, 9);
        g.AddEdge(2, 4, 14);
        g.AddEdge(4, 3, 7);
        g.AddEdge(3, 5, 20);
        g.AddEdge(4, 5, 4);

        Dinic dinic(g);
        ExpectEq(dinic.MaxFlow(0, 5), 23, "directed_clrs_example");
    }

    {
        // Build via Dinic::AddEdge directly.
        Dinic dinic(4);
        dinic.AddEdge(0, 1, 3);
        dinic.AddEdge(0, 2, 2);
        dinic.AddEdge(1, 2, 5);
        dinic.AddEdge(1, 3, 2);
        dinic.AddEdge(2, 3, 3);

        ExpectEq(dinic.MaxFlow(0, 3), 5, "directed_manual_edges");
    }

    {
        // Undirected input should be treated as two directed edges per edge.
        AdjListGraph g(3, false);
        g.AddEdge(0, 1, 5);
        g.AddEdge(1, 2, 3);
        g.AddEdge(0, 2, 4);

        Dinic dinic(g);
        ExpectEq(dinic.MaxFlow(0, 2), 7, "undirected_as_bidirectional");
    }

    {
        // Disconnected graph has zero max flow.
        AdjListGraph g(4, true);
        g.AddEdge(0, 1, 3);
        g.AddEdge(1, 2, 4);
        // No edge to node 3 from this component.

        Dinic dinic(g);
        ExpectEq(dinic.MaxFlow(0, 3), 0, "disconnected_zero_flow");
    }

    {
        // Parallel edges should contribute to total capacity.
        Dinic dinic(3);
        dinic.AddEdge(0, 1, 2);
        dinic.AddEdge(0, 1, 3);
        dinic.AddEdge(1, 2, 4);

        ExpectEq(dinic.MaxFlow(0, 2), 4, "parallel_edges_capacity_sum");
    }

    {
        // Zero-capacity edges should be ignored.
        Dinic dinic(3);
        dinic.AddEdge(0, 1, 0);
        dinic.AddEdge(0, 2, 5);
        dinic.AddEdge(2, 1, 5);

        ExpectEq(dinic.MaxFlow(0, 1), 5, "zero_capacity_ignored");
    }

    return 0;
}
