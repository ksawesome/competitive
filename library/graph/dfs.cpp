// @name   DFS (recursive, all components)
// @prefix cpdfs
// @key    t d
// @desc   [inside solve] needs n, adj. Labels every vertex with its component id.
// @usage  // after it: comp[v] = component of v, ncomp = number of components
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

void context(int n, vector<vector<int>>& adj) {
    // @begin
    vector<int> comp(n, -1);  // comp[v] = component id, -1 = unvisited
    int ncomp = 0;
    auto dfs = [&](auto&& self, int u) -> void {
        comp[u] = ncomp;
        for (int v : adj[u]) {
            if (comp[v] == -1) self(self, v);
        }
    };
    for (int s = 0; s < n; s++) {
        if (comp[s] == -1) {
            dfs(dfs, s);
            ncomp++;
        }
    }
    // @end
}
