#pragma once
#include <array>
#include <vector>

#include "graph/adj_list_graph.h"
#include "io/input_parser.h"

struct GridGraph {
    AdjListGraph graph;
    std::vector<int> cell_to_vertex;
    std::vector<std::array<int, 2>> vertex_to_cell;
    int rows = 0;
    int cols = 0;
};

GridGraph GridToGraph(const io::GridProblem& problem);
int GridCellToVertex(const GridGraph& grid, int row, int col);
std::array<int, 2> GridVertexToCell(const GridGraph& grid, int vertex);
