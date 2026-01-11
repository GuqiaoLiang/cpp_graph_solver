#pragma once
#include <vector>

struct Edge {
    int to;
    int weight;
};

class Graph {
public:
    virtual ~Graph() = default;

    virtual int NumVertices() const = 0;
    virtual const std::vector<Edge>& Neighbors(int u) const = 0;
};
