#include "algorithms/mst/mst.h"
#include "ds/union_find.h"
#include <queue>
#include <vector>
#include <stdexcept>


#include <algorithm>

constexpr int INF = 1000000000;
using Priority_queue = std::priority_queue<std::tuple<int, int>, std::vector<std::tuple<int, int>>, std::greater<>>;


MSTResult PrimMST(const Graph& g, int start) {
    // throw run time exception
    if (g.IsDirected()) {
        throw std::runtime_error("Kruskal requires undirected graph");
    }

    // keep track of the distance to tree
    std::vector<int> dist(g.NumVertices(), INF);

    // keep track of parent 
    std::vector<int> parent(g.NumVertices(), -1);
    std::vector<bool> visited(g.NumVertices(), false);



    std::vector<MSTEdge> edges;
    int total_weight = 0;

    Priority_queue q;
    dist[start] = 0;
    q.push({0, start});

    while (!q.empty())
    {
        const auto [distance, node] = q.top();
        q.pop();

        if (visited[node]) {
            continue;
        }
        
        visited[node] = true;
        if (parent[node] != -1) {
            edges.push_back({parent[node], node, dist[node]});
            total_weight += dist[node];
        }
        

        for (const auto& [u, w] : g.Neighbors(node)) {
            if (dist[u] > w && !visited[u])
            {
                dist[u] = w;
                parent[u] = node;
                q.push({dist[u], u});
            }
            
           
        }
    }

    if (edges.size() != g.NumVertices() - 1) {
        throw std::runtime_error("Graph is not connected");
    }

    return {edges, total_weight};


}

static std::vector<MSTEdge> CollectEdges(const Graph& g) {
    std::vector<MSTEdge> edges;

    int n = g.NumVertices();

    for (int u = 0; u < n; ++u) {
        for (const auto& [v, w] : g.Neighbors(u)) {
            if (u < v) {  
                edges.push_back({u, v, w});
            }
        }
    }

    return edges;
}


MSTResult KruskalMST(const Graph& g) {

    if (g.IsDirected()) {
        throw std::runtime_error("Kruskal requires undirected graph");
    }

    std::vector<MSTEdge> edges = CollectEdges(g);

    std::sort(edges.begin(), edges.end(),
              [](const MSTEdge& a, const MSTEdge& b) {
                  return std::get<2>(a) < std::get<2>(b);
              });

    UnionFind uf(g.NumVertices());

    std::vector<MSTEdge> mst_edges;
    int total_weight = 0;

    for (const auto& [u, v, w] : edges) {
        if (!uf.Connected(u, v)) {
            uf.Union(u, v);
            mst_edges.push_back({u, v, w});
            total_weight += w;

            if (mst_edges.size() == g.NumVertices() - 1) {
                break;
            }
        }
    }

    if (mst_edges.size() != g.NumVertices() - 1) {
        throw std::runtime_error("Graph is not connected, MST does not exist.");
    }

    return {mst_edges, total_weight};
}
