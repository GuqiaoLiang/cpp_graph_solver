#pragma once
#include "graph/graph.h"
#include <vector>


struct ConnectedComponentResult {
    std::vector<int> cids;
    int components = 0;
};

// return a list of component ids
ConnectedComponentResult Connected_Component(const Graph& g);