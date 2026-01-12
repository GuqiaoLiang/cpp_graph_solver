#pragma once
#include <vector>

#include "graph/graph.h"

struct BFSResult {
    std::vector<int> dist;
    std::vector<int> parent;
};

// Output the distance from s to any node in the graph.
std::vector<int> GraphBFS(const Graph& g, int s);
BFSResult GraphBFSWithParent(const Graph& g, int s);

std::vector<int> DFSAll(const Graph& g);

