#pragma once
#include "graph/graph.h"
#include <vector>

class Dinic {
public:
    explicit Dinic(int n);
    explicit Dinic(const Graph& g);

    // 加一条容量为 cap 的有向边 u -> v
    void AddEdge(int u, int v, int cap);

    // 计算从 s 到 t 的最大流
    int MaxFlow(int s, int t);

private:
    struct Edge {
        int to;
        int cap;
        int rev;   // 反向边在 graph[to] 中的下标
    };

    int n_;
    std::vector<std::vector<Edge>> graph_;
    std::vector<int> level_;
    std::vector<int> iter_;   // 当前弧优化指针

    bool Bfs(int s, int t);
    int Dfs(int v, int t, int f);

    void BuildFromDirectedGraph(const Graph& g);

    void BuildFromUndirectedGraph(const Graph& g);
};
