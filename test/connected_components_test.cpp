#include <cstdlib>
#include <iostream>

#include "include/algorithms/connectivity/connected_components.h"
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
        AdjListGraph g(6, false);
        g.AddEdge(0, 1);
        g.AddEdge(1, 2);
        g.AddEdge(3, 4);

        ConnectedComponentResult res = Connected_Component(g);
        ExpectEq(res.components, 3, "components_count");
        ExpectTrue(res.cids[0] == res.cids[1] && res.cids[1] == res.cids[2],
                   "component_group_012");
        ExpectTrue(res.cids[3] == res.cids[4], "component_group_34");
        ExpectTrue(res.cids[5] != res.cids[0] && res.cids[5] != res.cids[3],
                   "component_group_5");
    }

    {
        AdjListGraph g(3, true);
        g.AddEdge(0, 1);
        bool threw = false;
        try {
            (void)Connected_Component(g);
        } catch (const std::logic_error&) {
            threw = true;
        }
        ExpectTrue(threw, "components_directed_throws");
    }

    return 0;
}
