#pragma once
#include "graph/graph.h"


/**
 * Given an undirected graph, check whether it contains cycle
 * @throw invaild input exception, if the input is a directed graph
 */
bool HasCycleUndirected(const Graph& g);