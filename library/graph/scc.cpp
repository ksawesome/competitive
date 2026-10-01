// @name   SCC (Tarjan)
// @prefix cpscc
// @key    t c
// @desc   [global] auto [k, comp] = scc(adj). Ids are reverse-topological: edge u->v across SCCs has comp[u] > comp[v].
// @usage  auto [k, comp] = scc(adj);  // k components, comp[v] in [0, k)
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
pair<int, vector<int>> scc(const vector<vector<int>>& adj) {
    int n = adj.size(), timer = 0, ncomp = 0;
    vector<int> tin(n, -1), low(n), comp(n, -1), stk;
    auto dfs = [&](auto&& self, int u) -> void {
        tin[u] = low[u] = timer++;
        stk.push_back(u);
        for (int v : adj[u]) {
            if (tin[v] == -1) {
                self(self, v);
                low[u] = min(low[u], low[v]);
            } else if (comp[v] == -1) {
                low[u] = min(low[u], tin[v]);
            }
        }
        if (low[u] == tin[u]) {
            while (true) {
                int v = stk.back();
                stk.pop_back();
                comp[v] = ncomp;
                if (v == u) break;
            }
            ncomp++;
        }
    };
    for (int v = 0; v < n; v++)
        if (tin[v] == -1) dfs(dfs, v);
    return {ncomp, comp};
}
// @end
