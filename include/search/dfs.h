#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "search/problem.h"
#include "search/result.h"

namespace search {

template <typename State>
SearchResult<State> DFSFindOne(const SearchProblem<State>& problem,
                               const State& start);

template <typename State>
CountResult<State> DFSCountSolutions(const SearchProblem<State>& problem,
                                     const State& start);

}  // namespace search
