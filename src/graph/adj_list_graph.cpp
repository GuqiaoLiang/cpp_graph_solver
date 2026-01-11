#include "include/graph/adj_list_graph.h"

void AdjListGraph::AddEdge(int u, int v, int w = 1) {
    adj_[u].push_back({v, w});

    if (!directed_) {
        adj_[v].push_back({u, w});
    }
}

int AdjListGraph::NumVertices() const {
    return adj_.size();
}


const std::vector<Edge>& AdjListGraph::Neighbors(int u) const {
    return adj_[u];
}


std::unique_ptr<Graph> AdjListGraph::Clone() const {
    return std::make_unique<AdjListGraph>(*this); // copy ctor
}

bool AdjListGraph::IsDirected() const {
    return directed_;
}
