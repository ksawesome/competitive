// @name   Gaussian elimination mod p (linear systems, rank)
// @prefix cpgauss
// @key    u n
// @desc   [global] gauss_mod(a, b, x): solves a x = b (mod MOD, MOD prime) for an n x m matrix a. Returns the rank, or -1 if there is no solution; x receives one solution (free variables set to 0).
// @usage  vector<vector<ll>> a(n, vector<ll>(m)); vector<ll> b(n), x;
// @usage  int r = gauss_mod(a, b, x);   // -1: inconsistent; else m - r free variables, MOD^(m - r) solutions
// @complexity O(n m min(n, m))
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll MOD = 1000000007;

// @begin
int gauss_mod(vector<vector<ll>> a, vector<ll> b, vector<ll>& x) {
    int n = a.size(), m = n ? a[0].size() : 0, rank = 0;
    auto power = [](ll base, ll e) {
        ll r = 1;
        for (base %= MOD; e; e >>= 1, base = base * base % MOD)
            if (e & 1) r = r * base % MOD;
        return r;
    };
    for (int i = 0; i < n; i++) {
        for (ll& v : a[i]) v = (v % MOD + MOD) % MOD;
        b[i] = (b[i] % MOD + MOD) % MOD;
    }
    vector<int> where(m, -1);  // where[c]: the row whose pivot lies in column c
    for (int c = 0; c < m && rank < n; c++) {
        int piv = rank;
        while (piv < n && a[piv][c] == 0) piv++;
        if (piv == n) continue;  // no pivot: x[c] is a free variable
        swap(a[piv], a[rank]), swap(b[piv], b[rank]);
        ll inv = power(a[rank][c], MOD - 2);  // scale the pivot row to a leading 1
        for (int k = c; k < m; k++) a[rank][k] = a[rank][k] * inv % MOD;
        b[rank] = b[rank] * inv % MOD;
        for (int r = 0; r < n; r++) {  // clear column c in every other row
            ll f = a[r][c];
            if (r == rank || f == 0) continue;
            for (int k = c; k < m; k++) a[r][k] = (a[r][k] + (MOD - f) * a[rank][k]) % MOD;
            b[r] = (b[r] + (MOD - f) * b[rank]) % MOD;
        }
        where[c] = rank++;
    }
    for (int r = rank; r < n; r++)
        if (b[r] != 0) return -1;  // a row 0 = b[r] with b[r] != 0
    x.assign(m, 0);
    for (int c = 0; c < m; c++)
        if (where[c] != -1) x[c] = b[where[c]];
    return rank;
}
// @end
