// @name   Tree knapsack (merging child DPs)
// @prefix cptreeknap
// @key    y shift+k
// @desc   [inside solve] needs n, adj, root, w. best[v][j] = max value choosing j vertices of v's subtree, each chosen vertex's parent chosen too.
// @usage  // answer for exactly k vertices containing the root: best[root][k]; merging costs O(n^2) in total
// @complexity O(n^2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n, vector<vector<int>>& adj, int root, vector<ll>& w) {
    // @begin
    vector<int> par(n, -1), order{root};
    order.reserve(n);
    for (int i = 0; i < (int)order.size(); i++)
        for (int v : adj[order[i]])
            if (v != par[order[i]]) par[v] = order[i], order.push_back(v);
    const ll NONE = LLONG_MIN;
    vector<vector<ll>> best(n);
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        best[u] = {0, w[u]};  // j = 0: nothing, j = 1: only u
        for (int c : adj[u]) {
            if (c == par[u]) continue;
            vector<ll> merged(best[u].size() + best[c].size() - 1, NONE);
            for (int a = 0; a < (int)best[u].size(); a++) {
                if (best[u][a] == NONE) continue;
                for (int b = 0; b < (int)best[c].size(); b++) {
                    if (best[c][b] == NONE || (a == 0 && b > 0)) continue;  // c's picks need u picked
                    merged[a + b] = max(merged[a + b], best[u][a] + best[c][b]);
                }
            }
            best[u].swap(merged);
            vector<ll>().swap(best[c]);
        }
    }
    // @end
}
