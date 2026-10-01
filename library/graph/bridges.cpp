// @name   Bridges + articulation points
// @prefix cpbridges
// @key    t shift+b
// @desc   [global] auto [bridge_ids, cut_vertices] = bridges_and_cuts(n, edges); undirected, multi-edges OK.
// @usage  vector<pair<int, int>> edges;  // undirected {u, v}
// @usage  auto [bridge_ids, cut_vertices] = bridges_and_cuts(n, edges);
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
pair<vector<int>, vector<int>> bridges_and_cuts(int n, const vector<pair<int, int>>& edges) {
    vector<vector<pair<int, int>>> g(n);  // {neighbor, edge id}
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v] = edges[i];
        g[u].push_back({v, i});
        g[v].push_back({u, i});
    }
    vector<int> tin(n, -1), low(n), bridges;
    vector<bool> is_cut(n);
    int timer = 0;
    auto dfs = [&](auto&& self, int u, int parent_edge) -> void {
        tin[u] = low[u] = timer++;
        int children = 0;
        for (auto [v, id] : g[u]) {
            if (id == parent_edge) continue;
            if (tin[v] != -1) {
                low[u] = min(low[u], tin[v]);
                continue;
            }
            self(self, v, id);
            children++;
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) bridges.push_back(id);
            if (parent_edge != -1 && low[v] >= tin[u]) is_cut[u] = true;
        }
        if (parent_edge == -1 && children > 1) is_cut[u] = true;
    };
    for (int v = 0; v < n; v++)
        if (tin[v] == -1) dfs(dfs, v, -1);
    vector<int> cuts;
    for (int v = 0; v < n; v++)
        if (is_cut[v]) cuts.push_back(v);
    return {bridges, cuts};
}
// @end
