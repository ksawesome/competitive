// @name   Read weighted graph (n, m, edges u v w)
// @prefix cpgraphw
// @key    t shift+e
// @desc   [inside solve] reads n, m and m 1-indexed weighted edges into adj[u] = {v, w}, 0-indexed, undirected.
// @usage  // then: auto [dist, par] = dijkstra(adj, 0);
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context() {
    // @begin
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, ll>>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        u--, v--;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});  // delete for a directed graph
    }
    // @end
}
