#pragma once
#include "graph/graph.h" 
#include <vector>
#include <tuple>



namespace algorithms::undirected {

/**
 * An edge in MST: (u, v, weight)
 */
using MSTEdge = std::tuple<int, int, double>;

/**
 * Result of MST computation
 */
struct MSTResult {
    std::vector<MSTEdge> edges;
    double total_weight = 0.0;
};

/**
 * Kruskal's algorithm for Minimum Spanning Tree.
 * Graph must be undirected and connected (or it will return a forest).
 */
MSTResult KruskalMST(const Graph& g);

/**
 * Prim's algorithm for Minimum Spanning Tree.
 * start: starting vertex, default 0.
 */
MSTResult PrimMST(const Graph& g, int start = 0);

} // namespace algorithms::undirected
