#include <cstdlib>
#include <iostream>

#include "include/algorithms/mst/mst.h"
#include "include/graph/adj_list_graph.h"

namespace {
void ExpectTrue(bool value, const char* test_name) {
    if (!value) {
        std::cerr << "[FAIL] " << test_name << ": expected true, got false\n";
        std::exit(1);
    }
    std::cout << "[PASS] " << test_name << "\n";
}

void ExpectEq(int actual, int expected, const char* test_name) {
    if (actual != expected) {
        std::cerr << "[FAIL] " << test_name << ": expected " << expected
                  << ", got " << actual << "\n";
        std::exit(1);
    }
    std::cout << "[PASS] " << test_name << "\n";
}
}  // namespace

int main() {
    {
        AdjListGraph g(4, false);
        g.AddEdge(0, 1, 1);
        g.AddEdge(0, 2, 4);
        g.AddEdge(1, 2, 2);
        g.AddEdge(1, 3, 5);
        g.AddEdge(2, 3, 1);

        MSTResult k = KruskalMST(g);
        ExpectEq(static_cast<int>(k.edges.size()), 3, "kruskal_edge_count");
        ExpectEq(k.total_weight, 4, "kruskal_total_weight");

        MSTResult p = PrimMST(g);
        ExpectEq(static_cast<int>(p.edges.size()), 3, "prim_edge_count");
        ExpectEq(p.total_weight, 4, "prim_total_weight");
    }

    {
        AdjListGraph g(4, false);
        g.AddEdge(0, 1, 1);
        g.AddEdge(2, 3, 1);
        bool threw_kruskal = false;
        try {
            (void)KruskalMST(g);
        } catch (const std::runtime_error&) {
            threw_kruskal = true;
        }
        ExpectTrue(threw_kruskal, "kruskal_disconnected_throws");

        bool threw_prim = false;
        try {
            (void)PrimMST(g);
        } catch (const std::runtime_error&) {
            threw_prim = true;
        }
        ExpectTrue(threw_prim, "prim_disconnected_throws");
    }

    return 0;
}
