#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace search {

template <typename State>
struct SearchResult {
    bool found = false;
    std::vector<State> path;
    size_t expanded = 0;
};

template <typename State>
struct CountResult {
    size_t count = 0;
    size_t expanded = 0;
    std::vector<State> example_path;
};

template <typename State>
std::vector<State> BuildPath(const State& goal,
                             const std::unordered_map<State, State>& parent) {
    std::vector<State> path;
    auto it = parent.find(goal);
    if (it == parent.end()) {
        return path;
    }
    State cur = goal;
    while (true) {
        path.push_back(cur);
        auto pit = parent.find(cur);
        if (pit == parent.end() || pit->second == cur) {
            break;
        }
        cur = pit->second;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

}  // namespace search
