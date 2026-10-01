// @name   Tree DFS (parent, depth, subtree size, Euler tour)
// @prefix cptree
// @key    t e
// @desc   [inside solve] needs n, adj, root. Subtree of u = Euler positions [tin[u], tout[u]).
// @usage  // after it: par, depth, sub (subtree size), tin/tout (Euler tour)
// @usage  // subtree of u = positions [tin[u], tout[u]) -> a range query on the tour
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

void context(int n, vector<vector<int>>& adj, int root) {
    // @begin
    vector<int> par(n, -1), depth(n), sub(n, 1), tin(n), tout(n);
    int timer = 0;
    auto dfs = [&](auto&& self, int u) -> void {
        tin[u] = timer++;
        for (int v : adj[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            depth[v] = depth[u] + 1;
            self(self, v);
            sub[u] += sub[v];
        }
        tout[u] = timer;
    };
    dfs(dfs, root);
    auto is_ancestor = [&](int a, int b) { return tin[a] <= tin[b] && tout[b] <= tout[a]; };
    // @end
    (void)is_ancestor;
}
