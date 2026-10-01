// @name   N-th term of a linear recurrence (Kitamasa)
// @prefix cpkitamasa
// @key    u k
// @desc   [global] linear_rec_nth(a, c, n): a[n] for a[i] = c[0]*a[i-1] + ... + c[L-1]*a[i-L], given a[0..L-1]; mod MOD. Pairs with Berlekamp-Massey.
// @usage  ll f = linear_rec_nth({0, 1}, {1, 1}, 1'000'000'000'000'000'000LL);   // Fibonacci
// @complexity O(L^2 log n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
ll linear_rec_nth(const vector<ll>& a, const vector<ll>& c, ll n) {
    int L = c.size();
    if (L == 0) return 0;
    if (n < L) return (a[n] % MOD + MOD) % MOD;
    // work with polynomials mod f(x) = x^L - c[0] x^(L-1) - ... - c[L-1]; then a[n] = sum r_i a[i] for r = x^n mod f
    auto mul = [&](const vector<ll>& p, const vector<ll>& q) {
        vector<ll> r(2 * L - 1, 0);
        for (int i = 0; i < L; i++)
            if (p[i])
                for (int j = 0; j < L; j++) r[i + j] = (r[i + j] + p[i] * q[j]) % MOD;
        for (int i = 2 * L - 2; i >= L; i--)  // x^i = sum_j c[j] x^(i-1-j)
            if (r[i])
                for (int j = 0; j < L; j++) r[i - 1 - j] = (r[i - 1 - j] + r[i] * c[j]) % MOD;
        r.resize(L);
        return r;
    };
    vector<ll> res(L, 0), base(L, 0);
    res[0] = 1;
    if (L == 1) base[0] = (c[0] % MOD + MOD) % MOD;  // x = c[0] (mod x - c[0])
    else base[1] = 1;
    for (; n; n >>= 1, base = mul(base, base))
        if (n & 1) res = mul(res, base);
    ll ans = 0;
    for (int i = 0; i < L; i++) ans = (ans + res[i] * ((a[i] % MOD + MOD) % MOD)) % MOD;
    return ans;
}
// @end
