// @name   Read graph (n, m, m edges)
// @prefix cpgraph
// @key    t shift+g
// @desc   [inside solve] reads n, m and m 1-indexed edges into a 0-indexed undirected adjacency list.
// @usage  // then: dfs / bfs / ... over adj
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

void context() {
    // @begin
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);  // delete for a directed graph
    }
    // @end
}
