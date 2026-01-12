#pragma once

#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "search/problem.h"
#include "search/result.h"

namespace search {

template <typename State>
SearchResult<State> BFS(const SearchProblem<State>& problem, const State& start);

}  // namespace search
