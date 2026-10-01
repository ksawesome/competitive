// @name   Find a directed cycle
// @prefix cpcycle
// @key    t /
// @desc   [global] find_cycle(adj): vertices of some directed cycle in order (the last one has an edge back to the first); empty if acyclic.
// @usage  vector<int> cyc = find_cycle(adj);
// @usage  // undirected graphs: a DSU finds the first edge whose ends are already joined
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
vector<int> find_cycle(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> color(n), par(n, -1), cycle;  // color: 0 new, 1 on the DFS stack, 2 finished
    auto dfs = [&](auto&& self, int u) -> bool {
        color[u] = 1;
        for (int v : adj[u]) {
            if (color[v] == 0) {
                par[v] = u;
                if (self(self, v)) return true;
            } else if (color[v] == 1) {  // back edge u -> v closes a cycle v -> ... -> u -> v
                for (int x = u; x != v; x = par[x]) cycle.push_back(x);
                cycle.push_back(v);
                reverse(cycle.begin(), cycle.end());
                return true;
            }
        }
        color[u] = 2;
        return false;
    };
    for (int s = 0; s < n; s++)
        if (color[s] == 0 && dfs(dfs, s)) break;
    return cycle;
}
// @end
