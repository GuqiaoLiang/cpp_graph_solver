#pragma once

#include <cmath>
#include <vector>

#include "search/problem.h"

namespace search {

struct NQueensState {
    std::vector<int> cols;

    bool operator==(const NQueensState& other) const {
        return cols == other.cols;
    }
};

class NQueensProblem : public SearchProblem<NQueensState> {
public:
    explicit NQueensProblem(int n);

    bool IsGoal(const NQueensState& state) const override;

    std::vector<NQueensState> Expand(const NQueensState& state) const override;

    const NQueensState& Start() const { return start_; }
    int N() const { return n_; }

private:
    bool IsSafe(const NQueensState& state, int row, int col) const;

    int n_;
    NQueensState start_;
};

}  // namespace search

namespace std {

template <>
struct hash<search::NQueensState> {
    size_t operator()(const search::NQueensState& state) const noexcept {
        size_t h = 0;
        for (int col : state.cols) {
            size_t hc = std::hash<int>{}(col);
            h ^= hc + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

}  // namespace std
