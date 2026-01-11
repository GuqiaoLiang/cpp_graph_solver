#include "graph/graph.h"
#include <vector>

// a heuristic is a functor
// output the path from s to t
template <typename Heuristic>
std::vector<int> AStar(const Graph& g, int s, int t, Heuristic h);

