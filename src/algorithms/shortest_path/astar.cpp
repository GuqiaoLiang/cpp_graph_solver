#include "algorithms/shortest_path/astar.h"

#include <algorithm>

std::vector<int> ReconstructPath(const std::vector<int>& parent, int s, int t) {
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

    return {};
}
