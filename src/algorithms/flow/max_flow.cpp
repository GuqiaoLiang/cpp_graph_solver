#include "algorithms/flow/max_flow.h"

#include <queue>
#include <algorithm>

Dinic::Dinic(int n)
    : n_(n), graph_(n), level_(n), iter_(n) {}



Dinic::Dinic(const Graph& g)
    : n_(g.NumVertices()),
      graph_(n_),
      level_(n_),
      iter_(n_) {

    if (g.IsDirected()) {
        BuildFromDirectedGraph(g);
    } else {
        BuildFromUndirectedGraph(g);
    }
}

void Dinic::BuildFromDirectedGraph(const Graph& g) {
    for (int u = 0; u < n_; ++u) {
        for (const auto& [v, w] : g.Neighbors(u)) {
            AddEdge(u, v, w);   // the weight as capacity
        }
    }
}

void Dinic::BuildFromUndirectedGraph(const Graph& g) {
    for (int u = 0; u < n_; ++u) {
        for (const auto& [v, w] : g.Neighbors(u)) {
            if (u < v) {   // remove duplicate
                AddEdge(u, v, w);
                AddEdge(v, u, w);
            }
        }
    }
}









void Dinic::AddEdge(int u, int v, int cap) {
    Edge forward{v, cap, (int)graph_[v].size()};
    Edge backward{u, 0,   (int)graph_[u].size()};

    graph_[u].push_back(forward);
    graph_[v].push_back(backward);
}


bool Dinic::Bfs(int s, int t) {
    std::fill(level_.begin(), level_.end(), -1);

    std::queue<int> q;
    level_[s] = 0;
    q.push(s);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (const auto& e : graph_[v]) {
            if (e.cap > 0 && level_[e.to] < 0) {
                level_[e.to] = level_[v] + 1;
                q.push(e.to);
            }
        }
    }

    return level_[t] >= 0;  // 是否还能到达汇点
}

int Dinic::Dfs(int v, int t, int f) {
    if (v == t) return f;

    for (int& i = iter_[v]; i < graph_[v].size(); ++i) {
        Edge& e = graph_[v][i];

        if (e.cap > 0 && level_[e.to] == level_[v] + 1) {
            int pushed = Dfs(e.to, t, std::min(f, e.cap));
            if (pushed > 0) {
                e.cap -= pushed;
                graph_[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }
    }
    return 0;
}

int Dinic::MaxFlow(int s, int t) {
    int flow = 0;
    const int INF = 1e9;

    while (Bfs(s, t)) {
        std::fill(iter_.begin(), iter_.end(), 0);
        int pushed;
        while ((pushed = Dfs(s, t, INF)) > 0) {
            flow += pushed;
        }
    }
    return flow;
}
