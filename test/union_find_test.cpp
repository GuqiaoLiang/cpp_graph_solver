#include <cstdlib>
#include <iostream>

#include "include/ds/union_find.h"

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
        UnionFind uf(5);
        ExpectEq(uf.SizeOf(0), 1, "size_initial");
        ExpectTrue(!uf.Connected(0, 1), "initial_disconnected");

        uf.Union(0, 1);
        uf.Union(1, 2);
        ExpectTrue(uf.Connected(0, 2), "connected_after_union");
        ExpectEq(uf.SizeOf(0), 3, "size_after_union");
    }

    {
        UnionFind uf(2);
        bool threw = false;
        try {
            (void)uf.Find(3);
        } catch (const std::out_of_range&) {
            threw = true;
        }
        ExpectTrue(threw, "out_of_range");
    }

    return 0;
}
