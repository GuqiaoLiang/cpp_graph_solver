#pragma once

#include <string>
#include <vector>

#include "io/input_parser.h"
#include "search/problem.h"

namespace search {

struct GridCell {
    int row = 0;
    int col = 0;

    bool operator==(const GridCell& other) const {
        return row == other.row && col == other.col;
    }
};

class GridSearchProblem : public SearchProblem<GridCell> {
public:
    explicit GridSearchProblem(const io::GridProblem& problem);

    bool IsGoal(const GridCell& state) const override;

    std::vector<GridCell> Expand(const GridCell& state) const override;

    const GridCell& Start() const { return start_; }
    const GridCell& Target() const { return target_; }

    bool IsWalkable(const GridCell& cell) const;

private:
    int rows_;
    int cols_;
    std::vector<std::string> map_;
    bool allow_diagonal_;
    GridCell start_;
    GridCell target_;
};

}  // namespace search

namespace std {

template <>
struct hash<search::GridCell> {
    size_t operator()(const search::GridCell& cell) const noexcept {
        size_t h1 = std::hash<int>{}(cell.row);
        size_t h2 = std::hash<int>{}(cell.col);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

}  // namespace std
