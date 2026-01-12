#include "algorithms/shortest_path/dijkstra.h"

#include "algorithms/shortest_path/astar.h"

DijkstraResult DijkstraShortestPath(const Graph& g, int s) {
    auto res = AStarSearch(g, s, -1, [](int, int) { return 0LL; });
    return {std::move(res.dist), std::move(res.parent)};
}
