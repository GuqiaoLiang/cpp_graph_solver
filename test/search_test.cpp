#include <cstdlib>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "include/io/input_parser.h"
#include "include/search/astar.h"
#include "include/search/bfs.h"
#include "include/search/dfs.h"
#include "include/search/problems/grid_problem.h"
#include "include/search/problems/nqueens_problem.h"

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

void ExpectSize(size_t actual, size_t expected, const char* test_name) {
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
        io::GridProblem problem;
        problem.rows = 5;
        problem.cols = 5;
        problem.map = {
            "..#..",
            ".#.#.",
            "...#T",
            "##...",
            "S...."
        };
        problem.start = {4, 0};
        problem.target = {2, 4};
        problem.allow_diagonal = false;

        search::GridSearchProblem grid(problem);
        auto bfs = search::BFS(grid, grid.Start());
        ExpectTrue(bfs.found, "grid_bfs_found");
        ExpectEq(static_cast<int>(bfs.path.size()) - 1, 6, "grid_bfs_distance");

        std::function<long long(const search::GridCell&)> heuristic =
            [&](const search::GridCell& cell) -> long long {
            int dr = std::abs(cell.row - grid.Target().row);
            int dc = std::abs(cell.col - grid.Target().col);
            return dr + dc;
        };
        auto astar = search::AStarSearch(grid, grid.Start(), heuristic);
        ExpectTrue(astar.found, "grid_astar_found");
        ExpectEq(static_cast<int>(astar.path.size()) - 1, 6, "grid_astar_distance");
    }

    {
        search::NQueensProblem nqueens(4);
        auto res = search::DFSCountSolutions(nqueens, nqueens.Start());
        ExpectEq(static_cast<int>(res.count), 2, "nqueens_count");
        ExpectTrue(!res.example_path.empty(), "nqueens_example_path");
        ExpectSize(res.example_path.back().cols.size(), 4, "nqueens_solution_size");
    }

    return 0;
}
