#pragma once
#include "graph/graph.h"
#include <vector>

// Output the visited node in order
std::vector<int> GraphDFS_Iterative(const Graph& g, int s);

std::vector<int> GraphDFS_Recursive(const Graph& g, int s);

// Output the preorder of the graph 
std::vector<int> DFSAll(const Graph& g);