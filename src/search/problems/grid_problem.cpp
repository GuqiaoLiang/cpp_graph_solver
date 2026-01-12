#include "search/problems/grid_problem.h"

namespace search {

GridSearchProblem::GridSearchProblem(const io::GridProblem& problem)
    : rows_(problem.rows),
      cols_(problem.cols),
      map_(problem.map),
      allow_diagonal_(problem.allow_diagonal),
      start_{problem.start[0], problem.start[1]},
      target_{problem.target[0], problem.target[1]} {}

bool GridSearchProblem::IsGoal(const GridCell& state) const {
    return state.row == target_.row && state.col == target_.col;
}

std::vector<GridCell> GridSearchProblem::Expand(const GridCell& state) const {
    std::vector<GridCell> out;
    static const int dr4[4] = {-1, 1, 0, 0};
    static const int dc4[4] = {0, 0, -1, 1};
    static const int dr8[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    static const int dc8[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    if (allow_diagonal_) {
        for (int k = 0; k < 8; ++k) {
            GridCell next{state.row + dr8[k], state.col + dc8[k]};
            if (IsWalkable(next)) {
                out.push_back(next);
            }
        }
    } else {
        for (int k = 0; k < 4; ++k) {
            GridCell next{state.row + dr4[k], state.col + dc4[k]};
            if (IsWalkable(next)) {
                out.push_back(next);
            }
        }
    }
    return out;
}

bool GridSearchProblem::IsWalkable(const GridCell& cell) const {
    if (cell.row < 0 || cell.row >= rows_ || cell.col < 0 || cell.col >= cols_) {
        return false;
    }
    char ch = map_[cell.row][cell.col];
    return ch != '#';
}

}  // namespace search
