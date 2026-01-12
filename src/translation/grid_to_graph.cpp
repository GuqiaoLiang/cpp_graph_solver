#include "translation/grid_to_graph.h"

static bool IsWalkable(char cell) {
    return cell != '#';
}

GridGraph GridToGraph(const io::GridProblem& problem) {
    GridGraph out{
        AdjListGraph(problem.rows * problem.cols, false),
        std::vector<int>(problem.rows * problem.cols, -1),
        std::vector<std::array<int, 2>>(),
        problem.rows,
        problem.cols
    };

    int next_id = 0;
    for (int r = 0; r < problem.rows; ++r) {
        for (int c = 0; c < problem.cols; ++c) {
            if (IsWalkable(problem.map[r][c])) {
                out.cell_to_vertex[r * problem.cols + c] = next_id++;
            }
        }
    }

    out.graph = AdjListGraph(next_id, false);
    out.vertex_to_cell.resize(next_id);
    for (int r = 0; r < problem.rows; ++r) {
        for (int c = 0; c < problem.cols; ++c) {
            int v = GridCellToVertex(out, r, c);
            if (v >= 0) {
                out.vertex_to_cell[v] = { r, c };
            }
        }
    }

    const int dr4[4] = { -1, 1, 0, 0 };
    const int dc4[4] = { 0, 0, -1, 1 };
    const int dr8[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    const int dc8[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    const int* dr = problem.allow_diagonal ? dr8 : dr4;
    const int* dc = problem.allow_diagonal ? dc8 : dc4;
    int dirs = problem.allow_diagonal ? 8 : 4;

    for (int r = 0; r < problem.rows; ++r) {
        for (int c = 0; c < problem.cols; ++c) {
            int u = GridCellToVertex(out, r, c);
            if (u < 0) {
                continue;
            }
            for (int i = 0; i < dirs; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];
                if (nr < 0 || nr >= problem.rows || nc < 0 || nc >= problem.cols) {
                    continue;
                }
                int v = GridCellToVertex(out, nr, nc);
                if (v >= 0) {
                    out.graph.AddEdge(u, v, 1);
                }
            }
        }
    }

    return out;
}

int GridCellToVertex(const GridGraph& grid, int row, int col) {
    if (row < 0 || row >= grid.rows || col < 0 || col >= grid.cols) {
        return -1;
    }
    return grid.cell_to_vertex[row * grid.cols + col];
}

std::array<int, 2> GridVertexToCell(const GridGraph& grid, int vertex) {
    if (vertex < 0 || vertex >= static_cast<int>(grid.vertex_to_cell.size())) {
        return { -1, -1 };
    }
    return grid.vertex_to_cell[vertex];
}
