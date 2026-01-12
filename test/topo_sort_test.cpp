#include <cstdlib>
#include <iostream>
#include <vector>

#include "include/algorithms/dag/is_dag.h"
#include "include/algorithms/dag/topo_sort.h"
#include "include/graph/adj_list_graph.h"

namespace {
void ExpectTrue(bool value, const char* test_name) {
    if (!value) {
        std::cerr << "[FAIL] " << test_name << ": expected true, got false\n";
        std::exit(1);
    }
    std::cout << "[PASS] " << test_name << "\n";
}

void ExpectFalse(bool value, const char* test_name) {
    if (value) {
        std::cerr << "[FAIL] " << test_name << ": expected false, got true\n";
        std::exit(1);
    }
    std::cout << "[PASS] " << test_name << "\n";
}

bool IsTopologicalOrder(const Graph& g, const std::vector<int>& order) {
    if (static_cast<int>(order.size()) != g.NumVertices()) {
        return false;
    }
    std::vector<int> pos(g.NumVertices(), -1);
    for (int i = 0; i < g.NumVertices(); ++i) {
        int v = order[i];
        if (v < 0 || v >= g.NumVertices()) {
            return false;
        }
        pos[v] = i;
    }
    for (int u = 0; u < g.NumVertices(); ++u) {
        for (const auto& [v, w] : g.Neighbors(u)) {
            if (pos[u] >= pos[v]) {
                return false;
            }
        }
    }
    return true;
}
}  // namespace

int main() {
    {
        AdjListGraph g(6, true);
        g.AddEdge(5, 2);
        g.AddEdge(5, 0);
        g.AddEdge(4, 0);
        g.AddEdge(4, 1);
        g.AddEdge(2, 3);
        g.AddEdge(3, 1);

        ExpectTrue(IsDAG(g), "is_dag_true");
        std::vector<int> order = TopologicalSort(g);
        ExpectTrue(IsTopologicalOrder(g, order), "topo_order_valid");
    }

    {
        AdjListGraph g(3, true);
        g.AddEdge(0, 1);
        g.AddEdge(1, 2);
        g.AddEdge(2, 0);
        ExpectFalse(IsDAG(g), "is_dag_false_cycle");
    }

    {
        AdjListGraph g(2, false);
        g.AddEdge(0, 1);
        ExpectFalse(IsDAG(g), "is_dag_undirected_false");
    }

    return 0;
}
