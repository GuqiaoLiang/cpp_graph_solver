#include "search/bfs.h"

#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "search/problems/grid_problem.h"
#include "search/result.h"

namespace search {

template <typename State>
SearchResult<State> BFS(const SearchProblem<State>& problem, const State& start) {
    std::queue<State> q;
    std::unordered_set<State> visited;
    std::unordered_map<State, State> parent;

    q.push(start);
    visited.insert(start);
    parent[start] = start;

    size_t expanded = 0;
    while (!q.empty()) {
        State cur = q.front();
        q.pop();
        ++expanded;

        if (problem.IsGoal(cur)) {
            return SearchResult<State>{true, BuildPath(cur, parent), expanded};
        }

        for (const auto& next : problem.Expand(cur)) {
            if (visited.insert(next).second) {
                parent[next] = cur;
                q.push(next);
            }
        }
    }

    return SearchResult<State>{false, {}, expanded};
}

template SearchResult<GridCell> BFS(const SearchProblem<GridCell>& problem,
                                    const GridCell& start);

}  // namespace search
