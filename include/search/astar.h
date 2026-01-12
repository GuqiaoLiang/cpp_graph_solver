#pragma once

#include <functional>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "search/problem.h"
#include "search/result.h"

namespace search {

template <typename State>
SearchResult<State> AStarSearch(
    const SearchProblem<State>& problem,
    const State& start,
    std::function<long long(const State&)> heuristic);

}  // namespace search
