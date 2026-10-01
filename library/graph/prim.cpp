// @name   Prim MST (dense graphs)
// @prefix cpprim
// @key    t shift+5
// @desc   [global] prim_dense(w): MST weight for an n x n weight matrix (complete graphs, e.g. points in the plane). O(n^2).
// @usage  vector<vector<ll>> w(n, vector<ll>(n));  // w[i][j] = cost of edge i-j
// @usage  ll cost = prim_dense(w);
// @complexity O(n^2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll prim_dense(const vector<vector<ll>>& w) {  // the graph must be connected
    int n = w.size();
    vector<ll> best(n, LLONG_MAX);
    vector<bool> used(n);
    best[0] = 0;
    ll total = 0;
    for (int it = 0; it < n; it++) {
        int u = -1;
        for (int v = 0; v < n; v++)
            if (!used[v] && (u == -1 || best[v] < best[u])) u = v;
        used[u] = true;
        total += best[u];
        for (int v = 0; v < n; v++)
            if (!used[v] && w[u][v] < best[v]) best[v] = w[u][v];
    }
    return total;
}
// @end
