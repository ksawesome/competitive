// @name   Bitmask DP (travelling salesman)
// @prefix cptsp
// @key    t ]
// @desc   [global] tsp(dist): shortest cycle through all n <= ~18 vertices from 0. dp[mask][v] = best path covering mask, ending at v.
// @usage  ll best = tsp(dist);  // dist[i][j] = cost i -> j
// @usage  // Hamiltonian path count: same loops with dp[mask | 1 << v][v] += dp[mask][u] over edges
// @complexity O(2^n n^2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll tsp(const vector<vector<ll>>& dist) {
    int n = dist.size();
    if (n == 1) return 0;
    const ll inf = LLONG_MAX / 4;
    vector<vector<ll>> dp(1 << n, vector<ll>(n, inf));
    dp[1][0] = 0;
    for (int mask = 1; mask < (1 << n); mask += 2)  // masks containing vertex 0
        for (int u = 0; u < n; u++) {
            if (!(mask >> u & 1) || dp[mask][u] >= inf) continue;
            for (int v = 0; v < n; v++)
                if (!(mask >> v & 1)) dp[mask | 1 << v][v] = min(dp[mask | 1 << v][v], dp[mask][u] + dist[u][v]);
        }
    ll best = inf;
    for (int u = 1; u < n; u++) best = min(best, dp[(1 << n) - 1][u] + dist[u][0]);
    return best;
}
// @end
