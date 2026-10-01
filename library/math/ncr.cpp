// @name   nCr / factorials mod p
// @prefix cpncr
// @key    t n
// @desc   [global] init_comb(N) once, then nCr, nPr, catalan, stars_and_bars, fact[i], inv_fact[i]. Uses the template's MOD.
// @usage  init_comb(200000);
// @usage  nCr(n, r); nPr(n, r); catalan(n); fact[n];
// @complexity O(N) init, O(1) per query
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
vector<ll> fact, inv_fact;
void init_comb(int N) {
    fact.assign(N + 1, 1);
    inv_fact.assign(N + 1, 1);
    for (int i = 1; i <= N; i++) fact[i] = fact[i - 1] * i % MOD;
    ll b = fact[N], e = MOD - 2, r = 1;  // inv_fact[N] = fact[N]^(MOD-2)
    for (; e > 0; e >>= 1, b = b * b % MOD)
        if (e & 1) r = r * b % MOD;
    inv_fact[N] = r;
    for (int i = N; i > 0; i--) inv_fact[i - 1] = inv_fact[i] * i % MOD;
}
ll nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}
ll nPr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * inv_fact[n - r] % MOD;
}
ll catalan(int n) { return fact[2 * n] * inv_fact[n] % MOD * inv_fact[n + 1] % MOD; }  // needs init_comb(>= 2n + 1)
ll stars_and_bars(int n, int k) { return nCr(n + k - 1, k - 1); }  // ways to split n identical items into k boxes
// @end
