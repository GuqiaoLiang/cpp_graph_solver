#include "algorithms/astar.h"
#include <vector>
#include <algorithm>
#include <queue>


constexpr long long INF = 1e18;

using PriorityQueue = std::priority_queue< std::pair<long long, int>,
        std::vector<std::pair<long long, int>>,
        std::greater<>>;

template <typename Heuristic>
std::vector<int> AStar(const Graph& g, int s, int t, Heuristic h) {
    std::vector<long long> dist(g.NumVertices(), INF);

    std::vector<int> parent(g.NumVertices(), -1);

    PriorityQueue pq;
    dist[s] = 0;

    pq.push({h(s, t), s});

    while (!pq.empty())
    {
        auto [estimated_distance, curr] = pq.top();

        if (dist[curr] == INF) continue;  // avoiding overflow

        pq.pop();

        if (estimated_distance > dist[curr] + h(curr, t)) {
            continue; // skip out stated states
        }

        if (curr == t) {
            break;
        }

        for (const auto& [u, w] : g.Neighbors(curr)) {
            if (dist[u] > dist[curr] + w) {
                dist[u] = dist[curr] + w;
                parent[u] = curr;
                pq.push({dist[u] + h(u, t), u});
            }
        }
    }

    return pathHelper(parent, s, t);

}


std::vector<int> pathHelper(std::vector<int>& parent, int s, int t) {

    std::vector<int> path;

    int cur = t;
    while (cur != -1) {
        path.push_back(cur);
        if (cur == s) {
            std::reverse(path.begin(), path.end());
            return path;
        }   
        cur = parent[cur];
    }

    return {};  // unreachable
}