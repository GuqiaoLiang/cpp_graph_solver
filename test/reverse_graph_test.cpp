#include <cstdlib>
#include <iostream>

#include "include/graph/adj_list_graph.h"
#include "include/utils/reverse_graph.h"

namespace {
void ExpectTrue(bool value, const char* test_name) {
    if (!value) {
        std::cerr << "[FAIL] " << test_name << ": expected true, got false\n";
        std::exit(1);
    }
    std::cout << "[PASS] " << test_name << "\n";
}

bool HasEdge(const Graph& g, int u, int v, int w) {
    for (const auto& [to, weight] : g.Neighbors(u)) {
        if (to == v && weight == w) {
            return true;
        }
    }
    return false;
}
}  // namespace

int main() {
    AdjListGraph g(3, true);
    g.AddEdge(0, 1, 2);
    g.AddEdge(0, 2, 3);
    g.AddEdge(2, 1, 4);

    AdjListGraph rg = ReverseGraph(g);

    ExpectTrue(HasEdge(rg, 1, 0, 2), "reverse_edge_1");
    ExpectTrue(HasEdge(rg, 2, 0, 3), "reverse_edge_2");
    ExpectTrue(HasEdge(rg, 1, 2, 4), "reverse_edge_3");
    ExpectTrue(rg.IsDirected(), "reverse_graph_directed");

    return 0;
}
