// @name   Hungarian algorithm (min-cost assignment)
// @prefix cphungarian
// @key    u shift+h
// @desc   [global] hungarian(a): a is n x m with n <= m; assigns each row a distinct column minimizing the total cost. Returns {cost, col_of_row}. For maximum weight, negate the costs.
// @usage  auto [cost, col] = hungarian(a);   // row i -> column col[i]
// @complexity O(n^2 m)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
pair<ll, vector<int>> hungarian(const vector<vector<ll>>& a) {
    int n = a.size(), m = a[0].size();  // requires n <= m
    const ll BIG = LLONG_MAX / 4;
    // potentials u (rows), v (columns): a[i][j] - u[i] - v[j] >= 0, tight on matched pairs; 1-indexed, column 0 is virtual
    vector<ll> u(n + 1, 0), v(m + 1, 0);
    vector<int> p(m + 1, 0), way(m + 1, 0);  // p[j] = row matched to column j
    for (int i = 1; i <= n; i++) {
        p[0] = i;
        int j0 = 0;
        vector<ll> minv(m + 1, BIG);
        vector<char> used(m + 1, 0);
        do {  // Dijkstra-like search for the cheapest augmenting path from row i
            used[j0] = 1;
            int i0 = p[j0], j1 = 0;
            ll delta = BIG;
            for (int j = 1; j <= m; j++)
                if (!used[j]) {
                    ll cur = a[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) minv[j] = cur, way[j] = j0;
                    if (minv[j] < delta) delta = minv[j], j1 = j;
                }
            for (int j = 0; j <= m; j++)
                if (used[j]) u[p[j]] += delta, v[j] -= delta;
                else minv[j] -= delta;
            j0 = j1;
        } while (p[j0] != 0);
        do {  // flip the path
            int j1 = way[j0];
            p[j0] = p[j1], j0 = j1;
        } while (j0);
    }
    vector<int> col(n, -1);
    ll cost = 0;
    for (int j = 1; j <= m; j++)
        if (p[j]) col[p[j] - 1] = j - 1, cost += a[p[j] - 1][j - 1];
    return {cost, col};
}
// @end
