#include "algorithms/bfs.h"
#include <vector>
#include <queue>


std::vector<int> GraphBFS(const Graph& g, int s) {

    std::vector<int> dist(g.NumVertices(), -1);
    std::queue<int> q;

    dist[s] = 0;
    q.push(s);

    while (! q.empty())
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
