// @name   Tree DP (subtree DP, iterative)
// @prefix cptreedp
// @key    t ;
// @desc   [inside solve] needs n, adj, root. Children before parents (reverse BFS order); example: maximum independent set.
// @usage  // answer: max(dp[root][0], dp[root][1]); change the two lines marked "transition"
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n, vector<vector<int>>& adj, int root) {
    // @begin
    vector<int> par(n, -1), order{root};
    order.reserve(n);
    for (int i = 0; i < (int)order.size(); i++)
        for (int v : adj[order[i]])
            if (v != par[order[i]]) par[v] = order[i], order.push_back(v);
    vector<array<ll, 2>> dp(n);  // dp[v][0/1] = best in v's subtree with v not taken / taken
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        dp[u] = {0, 1};
        for (int v : adj[u]) {
            if (v == par[u]) continue;
            dp[u][0] += max(dp[v][0], dp[v][1]);  // transition
            dp[u][1] += dp[v][0];                 // transition
        }
    }
    // @end
}
