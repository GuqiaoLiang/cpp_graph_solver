#pragma once
#include <vector>

#include "graph/graph.h"

struct DijkstraResult {
    std::vector<long long> dist;
    std::vector<int> parent;
};

DijkstraResult DijkstraShortestPath(const Graph& g, int s);
