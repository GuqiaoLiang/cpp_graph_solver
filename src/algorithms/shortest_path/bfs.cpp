#include "algorithms/shortest_path/bfs.h"
#include <vector>
#include <queue>

std::vector<int> GraphBFS(const Graph& g, int s) {

    std::vector<int> dist(g.NumVertices(), -1);
    std::queue<int> q;

    dist[s] = 0;
    q.push(s);

    while (!q.empty())
    {
        int node = q.front();
        q.pop();

        for (const auto& [u, w] : g.Neighbors(node)) {
            if (dist[u] == -1) {
                dist[u] = dist[node] + 1;
                q.push(u);
            }
        }
    }

    return dist;
}

BFSResult GraphBFSWithParent(const Graph& g, int s) {
    BFSResult res;
    res.dist.assign(g.NumVertices(), -1);
    res.parent.assign(g.NumVertices(), -1);

    std::queue<int> q;
    res.dist[s] = 0;
    q.push(s);

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (const auto& [u, w] : g.Neighbors(node)) {
            if (res.dist[u] == -1) {
                res.dist[u] = res.dist[node] + 1;
                res.parent[u] = node;
                q.push(u);
            }
        }
    }

    return res;
}
