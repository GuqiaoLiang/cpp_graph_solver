#pragma once
#include "graph.h"

// a graph implemented by AdjList
class AdjListGraph : public Graph {
public:
    AdjListGraph(int n, bool directed = false)
        : adj_(n), directed_(directed) {}

    void AddEdge(int u, int v, int w = 1);

    int NumVertices() const;

    const std::vector<Edge>& Neighbors(int u) const override;

    std::unique_ptr<Graph> Clone() const override;

    bool IsDirected() const override;


private:
    std::vector<std::vector<Edge>> adj_;
    bool directed_;
};
