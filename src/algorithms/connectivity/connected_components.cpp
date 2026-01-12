#include "algorithms/connectivity/connected_components.h"
#include <stdexcept>

static void dfs_component_helper(const Graph& g, int s, int id,
                                 std::vector<int>& res);

// returns a list of component ids, starting from 0
ConnectedComponentResult Connected_Component(const Graph& g) {

    if (g.IsDirected()) {
        throw std::logic_error("Connected component can't be calculated on directed graph");
    }
    

    std::vector<int> res(g.NumVertices(), -1);

    int counter = 0;

    for (int i = 0; i < g.NumVertices(); i++) {
        // haven't been marked
        if (res[i] == -1) {
            dfs_component_helper(g, i, counter, res);
            counter++;
        }
    }

    return {res, counter};


}


void dfs_component_helper(const Graph& g, int s, int id, std::vector<int>& res) {

    res[s] = id;

    for (const auto& [v, w] : g.Neighbors(s)) {
        // haven't been marked with id
        if (res[v] == -1) {
           dfs_component_helper(g, v, id, res);
        }
    }

    return;

}
