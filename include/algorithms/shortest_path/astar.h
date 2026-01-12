#pragma once
#include <queue>
#include <utility>
#include <vector>

#include "graph/graph.h"

struct AStarResult {
    std::vector<long long> dist;
    std::vector<int> parent;
};

std::vector<int> ReconstructPath(const std::vector<int>& parent, int s, int t);

// Heuristic is a functor: h(u, t) -> estimate cost from u to t.
template <typename Heuristic>
AStarResult AStarSearch(const Graph& g, int s, int t, Heuristic h) {
    const long long INF = 1e18;

    AStarResult res;
    res.dist.assign(g.NumVertices(), INF);
    res.parent.assign(g.NumVertices(), -1);

    using Node = std::pair<long long, int>;
    std::priority_queue<Node, std::vector<Node>, std::greater<>> pq;

    res.dist[s] = 0;
    pq.push({h(s, t), s});

    while (!pq.empty()) {
        auto [estimated_distance, curr] = pq.top();
        pq.pop();

        if (res.dist[curr] == INF) {
            continue;
        }

        if (estimated_distance > res.dist[curr] + h(curr, t)) {
            continue;
        }

        if (t >= 0 && curr == t) {
            break;
        }

        for (const auto& [u, w] : g.Neighbors(curr)) {
            if (res.dist[u] > res.dist[curr] + w) {
                res.dist[u] = res.dist[curr] + w;
                res.parent[u] = curr;
                pq.push({res.dist[u] + h(u, t), u});
            }
        }
    }

    return res;
}
