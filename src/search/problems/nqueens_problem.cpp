#include "search/problems/nqueens_problem.h"

namespace search {

NQueensProblem::NQueensProblem(int n) : n_(n) {}

bool NQueensProblem::IsGoal(const NQueensState& state) const {
    return static_cast<int>(state.cols.size()) == n_;
}

std::vector<NQueensState> NQueensProblem::Expand(const NQueensState& state) const {
    std::vector<NQueensState> out;
    int row = static_cast<int>(state.cols.size());
    if (row >= n_) {
        return out;
    }
    for (int col = 0; col < n_; ++col) {
        if (IsSafe(state, row, col)) {
            NQueensState next = state;
            next.cols.push_back(col);
            out.push_back(std::move(next));
        }
    }
    return out;
}

bool NQueensProblem::IsSafe(const NQueensState& state, int row, int col) const {
    for (int r = 0; r < static_cast<int>(state.cols.size()); ++r) {
        int c = state.cols[r];
        if (c == col) {
            return false;
        }
        if (std::abs(row - r) == std::abs(col - c)) {
            return false;
        }
    }
    return true;
}

}  // namespace search
