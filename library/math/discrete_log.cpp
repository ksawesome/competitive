// @name   Discrete logarithm (baby-step giant-step)
// @prefix cpdlog
// @key    u l
// @desc   [global] discrete_log(a, b, m): smallest x >= 0 with a^x = b (mod m), or -1. Any m (not only primes), m <= 3e9.
// @usage  ll x = discrete_log(2, 5, 13);   // 2^9 = 512 = 5 (mod 13) -> 9
// @complexity O(sqrt(m)) time and memory
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll discrete_log(ll a, ll b, ll m) {
    a %= m, b %= m;
    if (m == 1) return 0;
    ll k = 1 % m, add = 0, g;
    while ((g = gcd(a, m)) > 1) {  // peel off common factors: a^x = b needs g | b once x >= 1
        if (b == k) return add;
        if (b % g) return -1;
        b /= g, m /= g, add++;
        k = k * (a / g) % m;
    }
    ll n = (ll)sqrtl((long double)m) + 1, an = 1;
    for (ll i = 0; i < n; i++) an = an * a % m;
    unordered_map<ll, ll> baby;  // b * a^q -> largest q (largest q gives the smallest x below)
    baby.reserve(2 * n);
    for (ll q = 0, cur = b; q <= n; q++, cur = cur * a % m) baby[cur] = q;
    for (ll p = 1, cur = k; p <= n; p++) {  // k * a^(n p) = b * a^q  ->  x = n p - q
        cur = cur * an % m;
        auto it = baby.find(cur);
        if (it != baby.end()) return n * p - it->second + add;
    }
    return -1;
}
// @end
