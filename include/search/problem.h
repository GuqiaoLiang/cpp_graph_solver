#pragma once

#include <vector>

namespace search {

template <typename State>
class SearchProblem {
public:
    virtual ~SearchProblem() = default;
    virtual bool IsGoal(const State& state) const = 0;
    virtual std::vector<State> Expand(const State& state) const = 0;
};

}  // namespace search
