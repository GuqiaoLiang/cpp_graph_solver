#include "search/astar.h"

#include <functional>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "search/problems/grid_problem.h"
#include "search/result.h"

namespace search {

template <typename State>
SearchResult<State> AStarSearch(const SearchProblem<State>& problem,
                                const State& start,
                                std::function<long long(const State&)> heuristic) {
    struct Node {
        State state;
        long long f = 0;
        long long g = 0;
    };
    struct Cmp {
        bool operator()(const Node& a, const Node& b) const {
            if (a.f != b.f) {
                return a.f > b.f;
            }
            return a.g > b.g;
        }
    };

    std::priority_queue<Node, std::vector<Node>, Cmp> open;
    std::unordered_map<State, long long> gscore;
    std::unordered_map<State, State> parent;
    std::unordered_set<State> closed;

    gscore[start] = 0;
    parent[start] = start;
    open.push(Node{start, heuristic(start), 0});

    size_t expanded = 0;
    while (!open.empty()) {
        Node cur = open.top();
        open.pop();

        if (!closed.insert(cur.state).second) {
            continue;
        }

        ++expanded;
        if (problem.IsGoal(cur.state)) {
            return SearchResult<State>{true, BuildPath(cur.state, parent), expanded};
        }

        for (const auto& next : problem.Expand(cur.state)) {
            if (closed.find(next) != closed.end()) {
                continue;
            }
            long long tentative = cur.g + 1;
            auto it = gscore.find(next);
            if (it == gscore.end() || tentative < it->second) {
                gscore[next] = tentative;
                parent[next] = cur.state;
                open.push(Node{next, tentative + heuristic(next), tentative});
            }
        }
    }

    return SearchResult<State>{false, {}, expanded};
}

template SearchResult<GridCell> AStarSearch(
    const SearchProblem<GridCell>& problem,
    const GridCell& start,
    std::function<long long(const GridCell&)> heuristic);

}  // namespace search
