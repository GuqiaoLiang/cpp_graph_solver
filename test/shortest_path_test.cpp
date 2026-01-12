#include <cstdlib>
#include <iostream>
#include <vector>

#include "include/algorithms/shortest_path/bfs.h"
#include "include/algorithms/shortest_path/dfs.h"
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

void ExpectVecEq(const std::vector<int>& actual,
                 const std::vector<int>& expected,
                 const char* test_name) {
    if (actual.size() != expected.size()) {
        std::cerr << "[FAIL] " << test_name << ": size mismatch\n";
        std::exit(1);
    }
    for (size_t i = 0; i < actual.size(); ++i) {
        if (actual[i] != expected[i]) {
            std::cerr << "[FAIL] " << test_name << ": mismatch at " << i
                      << " expected " << expected[i] << ", got " << actual[i]
                      << "\n";
            std::exit(1);
        }
    }
    std::cout << "[PASS] " << test_name << "\n";
}
}  // namespace

int main() {
    {
        AdjListGraph g(5, true);
        g.AddEdge(0, 1);
        g.AddEdge(0, 2);
        g.AddEdge(1, 2);
        g.AddEdge(2, 3);

        std::vector<int> dist = GraphBFS(g, 0);
        ExpectVecEq(dist, {0, 1, 1, 2, -1}, "bfs_distances");
    }

    {
        AdjListGraph g(5, true);
        g.AddEdge(0, 1);
        g.AddEdge(0, 2);
        g.AddEdge(1, 3);
        g.AddEdge(2, 4);

        std::vector<int> expected = {0, 1, 3, 2, 4};
        ExpectVecEq(GraphDFS_Recursive(g, 0), expected, "dfs_recursive_order");
        ExpectVecEq(GraphDFS_Iterative(g, 0), expected, "dfs_iterative_order");
    }

    {
        AdjListGraph g(6, false);
        g.AddEdge(0, 1);
        g.AddEdge(1, 2);
        g.AddEdge(3, 4);

        std::vector<int> order = DFSAll(g);
        ExpectEq(static_cast<int>(order.size()), 6, "dfs_all_size");
        ExpectVecEq(order, {0, 1, 2, 3, 4, 5}, "dfs_all_preorder");
    }

    return 0;
}
