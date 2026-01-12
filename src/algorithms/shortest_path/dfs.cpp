#include "algorithms/shortest_path/dfs.h"
#include <stack>

static void dfs_Recurse_Helper(const Graph& g, int s, std::vector<bool>& visited,
                               std::vector<int>& res);

// the order of visited node
std::vector<int> GraphDFS_Iterative(const Graph& g, int s) {

    std::vector<bool> visited(g.NumVertices(), false);
    std::vector<int> res;
    std::stack<int> q;

    q.push(s);

    while (!q.empty())
    {
        int curr = q.top();
        q.pop();
        if (visited[curr]) {
            continue;
        }
        visited[curr] = true;
        res.push_back(curr);
        

        const auto& neighbor = g.Neighbors(curr);
        // iterate through reverse order
        for (auto it = neighbor.rbegin(); it != neighbor.rend(); ++it) {
            if (!visited[it->to]) {
                q.push(it->to);
            }
        }
    }

    return res;
    
}

std::vector<int> GraphDFS_Recursive(const Graph& g, int s) {

    std::vector<bool> visited(g.NumVertices(), false);
    std::vector<int> res;

    dfs_Recurse_Helper(g, s, visited, res);

    return res;
}



void dfs_Recurse_Helper(const Graph& g, int s, std::vector<bool>& visited, std::vector<int>& res) {
    visited[s] = true;
    res.push_back(s);

    for (const auto& [u, w] : g.Neighbors(s)) {
        if (visited[u]) {
            continue;
        }

        dfs_Recurse_Helper(g, u, visited, res);
    }

    return;


}

// Output the preorder of the graph 
std::vector<int> DFSAll(const Graph& g) {
    std::vector<bool> visited(g.NumVertices(), false);

    std::vector<int> res;

    for (int i = 0; i < g.NumVertices(); i ++) {
        if (! visited[i]) {
            dfs_Recurse_Helper(g, i, visited, res);
        }
    }

    return res;

}
