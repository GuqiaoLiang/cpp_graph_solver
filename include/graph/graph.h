#pragma once
#include <vector>
#include <memory>

struct Edge {
    int to;
    int weight;
};

class Graph {
public:
    virtual ~Graph() = default;

    virtual int NumVertices() const = 0;
    virtual const std::vector<Edge>& Neighbors(int u) const = 0;

    virtual std::unique_ptr<Graph> Clone() const = 0;

    virtual bool IsDirected() const = 0; 
};
