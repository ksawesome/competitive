// @name   2-edge-connected components
// @prefix cp2ecc
// @key    y b
// @desc   [global] auto [k, comp] = two_edge_cc(n, edges): vertices stay connected after deleting any single edge iff same comp. Bridges join comps.
// @usage  auto [k, comp] = two_edge_cc(n, edges);
// @usage  // bridge tree: for each edge (u, v) with comp[u] != comp[v], connect comp[u] - comp[v]
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
pair<int, vector<int>> two_edge_cc(int n, const vector<pair<int, int>>& edges) {
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v] = edges[i];
        g[u].push_back({v, i});
        g[v].push_back({u, i});
    }
    vector<int> tin(n, -1), low(n), comp(n, -1), stk;
    int timer = 0, k = 0;
    auto dfs = [&](auto&& self, int u, int parent_edge) -> void {
        tin[u] = low[u] = timer++;
        stk.push_back(u);
        for (auto [v, id] : g[u]) {
            if (id == parent_edge) continue;
            if (tin[v] == -1) self(self, v, id), low[u] = min(low[u], low[v]);
            else low[u] = min(low[u], tin[v]);
        }
        if (low[u] == tin[u]) {  // the edge above u (if any) is a bridge: pop u's component
            for (int v = -1; v != u;) v = stk.back(), stk.pop_back(), comp[v] = k;
            k++;
        }
    };
    for (int v = 0; v < n; v++)
        if (tin[v] == -1) dfs(dfs, v, -1);
    return {k, comp};
}
// @end
