#include <cstdlib>
#include <iostream>

#include "include/algorithms/connectivity/cycle_detection.h"
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
}  // namespace

int main() {
    {
        AdjListGraph g(4, false);
        g.AddEdge(0, 1);
        g.AddEdge(1, 2);
        g.AddEdge(2, 3);
        ExpectFalse(HasCycleUndirected(g), "tree_no_cycle");
    }

    {
        AdjListGraph g(3, false);
        g.AddEdge(0, 1);
        g.AddEdge(1, 2);
        g.AddEdge(2, 0);
        ExpectTrue(HasCycleUndirected(g), "triangle_has_cycle");
    }

    {
        AdjListGraph g(2, true);
        g.AddEdge(0, 1);
        bool threw = false;
        try {
            (void)HasCycleUndirected(g);
        } catch (const std::logic_error&) {
            threw = true;
        }
        ExpectTrue(threw, "directed_throws");
    }

    return 0;
}
