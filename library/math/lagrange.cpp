// @name   Lagrange interpolation
// @prefix cplagrange
// @key    u i
// @desc   [global] lagrange_consecutive(y, x): the degree < k polynomial with P(i) = y[i] for i = 0..k-1, evaluated at any x; lagrange_general(xs, ys, x) for arbitrary distinct points. Mod MOD (prime).
// @usage  ll s = lagrange_consecutive({0, 1, 5, 14}, 1'000'000'000);   // 0^2 + 1^2 + ... + 10^9^2 mod MOD
// @usage  ll v = lagrange_general(xs, ys, x);
// @complexity O(k) consecutive points (plus O(log MOD)), O(k^2) general points
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
ll lag_pow(ll b, ll e) {
    ll r = 1;
    for (b %= MOD; e; e >>= 1, b = b * b % MOD)
        if (e & 1) r = r * b % MOD;
    return r;
}
ll lagrange_consecutive(const vector<ll>& y, ll x) {
    int k = y.size();
    x %= MOD;
    if (x < 0) x += MOD;
    if (x < k) return (y[x] % MOD + MOD) % MOD;
    // P(x) = sum_i y_i * prod_{j != i} (x - j) / (i - j)
    vector<ll> pre(k + 1, 1), suf(k + 1, 1), fact(k, 1);
    for (int i = 0; i < k; i++) pre[i + 1] = pre[i] * ((x - i) % MOD) % MOD;
    for (int i = k - 1; i >= 0; i--) suf[i] = suf[i + 1] * ((x - i) % MOD) % MOD;
    for (int i = 1; i < k; i++) fact[i] = fact[i - 1] * i % MOD;
    ll res = 0;
    for (int i = 0; i < k; i++) {
        ll num = pre[i] * suf[i + 1] % MOD;
        ll den = fact[i] * fact[k - 1 - i] % MOD;  // i! (k-1-i)!, sign (-1)^(k-1-i)
        ll term = (y[i] % MOD + MOD) % MOD * num % MOD * lag_pow(den, MOD - 2) % MOD;
        res = (k - 1 - i) % 2 ? (res - term + MOD) % MOD : (res + term) % MOD;
    }
    return res;
}
ll lagrange_general(const vector<ll>& xs, const vector<ll>& ys, ll x) {
    ll res = 0;
    int k = xs.size();
    for (int i = 0; i < k; i++) {
        ll num = 1, den = 1;
        for (int j = 0; j < k; j++)
            if (j != i) num = num * (((x - xs[j]) % MOD + MOD) % MOD) % MOD, den = den * (((xs[i] - xs[j]) % MOD + MOD) % MOD) % MOD;
        res = (res + (ys[i] % MOD + MOD) % MOD * num % MOD * lag_pow(den, MOD - 2)) % MOD;
    }
    return res;
}
// @end
