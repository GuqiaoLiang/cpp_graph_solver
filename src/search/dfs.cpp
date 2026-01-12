#include "search/dfs.h"

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "search/problems/grid_problem.h"
#include "search/problems/nqueens_problem.h"
#include "search/result.h"

namespace search {

template <typename State>
SearchResult<State> DFSFindOne(const SearchProblem<State>& problem,
                               const State& start) {
    std::vector<State> stack;
    std::unordered_set<State> visited;
    std::unordered_map<State, State> parent;

    stack.push_back(start);
    visited.insert(start);
    parent[start] = start;

    size_t expanded = 0;
    while (!stack.empty()) {
        State cur = stack.back();
        stack.pop_back();
        ++expanded;

        if (problem.IsGoal(cur)) {
            return SearchResult<State>{true, BuildPath(cur, parent), expanded};
        }

        for (const auto& next : problem.Expand(cur)) {
            if (visited.insert(next).second) {
                parent[next] = cur;
                stack.push_back(next);
            }
        }
    }

    return SearchResult<State>{false, {}, expanded};
}

template <typename State>
CountResult<State> DFSCountSolutions(const SearchProblem<State>& problem,
                                     const State& start) {
    CountResult<State> out;
    std::vector<State> path;
    path.push_back(start);

    std::function<void(const State&)> dfs = [&](const State& cur) {
        ++out.expanded;
        if (problem.IsGoal(cur)) {
            ++out.count;
            if (out.example_path.empty()) {
                out.example_path = path;
            }
            return;
        }

        for (const auto& next : problem.Expand(cur)) {
            path.push_back(next);
            dfs(next);
            path.pop_back();
        }
    };

    dfs(start);
    return out;
}

template SearchResult<GridCell> DFSFindOne(const SearchProblem<GridCell>& problem,
                                           const GridCell& start);
template SearchResult<NQueensState> DFSFindOne(
    const SearchProblem<NQueensState>& problem,
    const NQueensState& start);
template CountResult<NQueensState> DFSCountSolutions(
    const SearchProblem<NQueensState>& problem,
    const NQueensState& start);

}  // namespace search
