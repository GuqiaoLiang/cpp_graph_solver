#include "io/grid_parser.h"

#include <stdexcept>

namespace io {

static std::array<int, 2> ParsePoint(const nlohmann::json& value) {
    if (!value.is_array() || value.size() != 2) {
        throw std::runtime_error("Point must be [row, col]");
    }
    return { value.at(0).get<int>(), value.at(1).get<int>() };
}

GridProblem ParseGridProblem(const nlohmann::json& problem) {
    GridProblem out;
    out.rows = problem.at("rows").get<int>();
    out.cols = problem.at("cols").get<int>();
    out.map = problem.at("map").get<std::vector<std::string>>();
    out.allow_diagonal = problem.value("allow_diagonal", false);

    if (problem.contains("start")) {
        out.start = ParsePoint(problem.at("start"));
    }
    if (problem.contains("target")) {
        out.target = ParsePoint(problem.at("target"));
    }

    bool has_start = problem.contains("start");
    bool has_target = problem.contains("target");
    if (!has_start || !has_target) {
        for (int r = 0; r < out.rows; ++r) {
            for (int c = 0; c < out.cols; ++c) {
                if (out.map[r][c] == 'S') {
                    out.start = { r, c };
                    has_start = true;
                } else if (out.map[r][c] == 'T') {
                    out.target = { r, c };
                    has_target = true;
                }
            }
        }
    }

    if (!has_start || !has_target) {
        throw std::runtime_error("Grid start/target not specified");
    }

    return out;
}

}  // namespace io
