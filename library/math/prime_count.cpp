// @name   Prime counting and prime sums up to 1e11 (Lucy)
// @prefix cpprimecount
// @key    u p
// @desc   [global] prime_count(n) = number of primes <= n (exact); prime_sum(n) = sum of primes <= n mod MOD.
// @usage  ll pi = prime_count(100'000'000'000LL);   // 4118054813
// @usage  ll s = prime_sum(n);
// @complexity O(n^(3/4)) time, O(sqrt n) memory
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
// S(v) = sum of g(k) over 2 <= k <= v that survive sieving by the primes < p; g completely multiplicative
// (g = 1 counts primes, g(k) = k sums them). Only v = n / i matter: lo[v] for v <= r, hi[i] for v = n / i.
template <bool SUM>
ll lucy_sieve(ll n) {
    if (n < 2) return 0;
    auto g_upto = [](ll v) -> ll {  // sum of g(k) for 2 <= k <= v
        if (!SUM) return v - 1;
        ll a = v % 2 == 0 ? (v / 2) % MOD : v % MOD, b = v % 2 == 0 ? (v + 1) % MOD : ((v + 1) / 2) % MOD;
        return (a * b - 1 + MOD) % MOD;
    };
    ll r = (ll)sqrtl((long double)n);
    while (r * r > n) r--;
    while ((r + 1) * (r + 1) <= n) r++;
    vector<ll> lo(r + 1), hi(r + 1);
    for (ll v = 1; v <= r; v++) lo[v] = g_upto(v), hi[v] = g_upto(n / v);
    for (ll p = 2; p <= r; p++) {
        if (lo[p] == lo[p - 1]) continue;  // p was sieved out: not prime
        ll below = lo[p - 1], gp = SUM ? p % MOD : 1, p2 = p * p;
        auto sub = [&](ll& s, ll part) {  // s -= g(p) * (part - S(p - 1)): remove numbers whose smallest prime is p
            ll delta = gp * (part - below);
            s = SUM ? ((s - delta) % MOD + MOD) % MOD : s - delta;
        };
        for (ll i = 1; i <= r && n / i >= p2; i++) {
            ll d = i * p;
            sub(hi[i], d <= r ? hi[d] : lo[n / d]);
        }
        for (ll v = r; v >= p2; v--) sub(lo[v], lo[v / p]);
    }
    return hi[1];
}
ll prime_count(ll n) { return lucy_sieve<false>(n); }
ll prime_sum(ll n) { return lucy_sieve<true>(n); }
// @end
