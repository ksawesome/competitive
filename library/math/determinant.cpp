// @name   Determinant mod p, matrix-tree theorem
// @prefix cpdet
// @key    u shift+d
// @desc   [global] det_mod(A): determinant mod MOD (prime) by Gaussian elimination; count_spanning_trees(n, edges): Kirchhoff's matrix-tree theorem (multi-edges allowed). LGV lemma: det of path-count matrix.
// @usage  ll d = det_mod(A);
// @usage  ll t = count_spanning_trees(n, edges);   // edges: 0-indexed pairs
// @complexity O(n^3)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
ll det_mod(vector<vector<ll>> a) {
    int n = a.size();
    auto power = [](ll b, ll e) {
        ll r = 1;
        for (b %= MOD; e; e >>= 1, b = b * b % MOD)
            if (e & 1) r = r * b % MOD;
        return r;
    };
    for (auto& row : a)
        for (ll& x : row) x = (x % MOD + MOD) % MOD;
    ll det = 1;
    for (int c = 0; c < n; c++) {
        int piv = c;
        while (piv < n && a[piv][c] == 0) piv++;
        if (piv == n) return 0;
        if (piv != c) swap(a[piv], a[c]), det = (MOD - det) % MOD;  // a row swap flips the sign
        det = det * a[c][c] % MOD;
        ll inv = power(a[c][c], MOD - 2);
        for (int r = c + 1; r < n; r++) {
            ll f = a[r][c] * inv % MOD;
            if (f)
                for (int k = c; k < n; k++) a[r][k] = (a[r][k] - f * a[c][k] % MOD + MOD) % MOD;
        }
    }
    return det;
}
ll count_spanning_trees(int n, const vector<pair<int, int>>& edges) {
    if (n == 1) return 1;
    vector<vector<ll>> lap(n - 1, vector<ll>(n - 1, 0));  // Laplacian without the last row and column
    for (auto [u, v] : edges) {
        if (u == v) continue;
        if (u < n - 1) lap[u][u]++;
        if (v < n - 1) lap[v][v]++;
        if (u < n - 1 && v < n - 1) lap[u][v]--, lap[v][u]--;
    }
    return det_mod(lap);
}
// @end
