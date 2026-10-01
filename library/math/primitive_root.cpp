// @name   Primitive root
// @prefix cpprimroot
// @key    u g
// @desc   [global] primitive_root(m): smallest generator g of (Z/mZ)* (exists for m = 1, 2, 4, p^k, 2p^k), else -1. m <= 3e9.
// @usage  ll g = primitive_root(998244353);   // 3
// @complexity O(sqrt(m) + answer * (number of prime factors of phi) * log m)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll primitive_root(ll m) {
    auto primes_of = [](ll x) {
        vector<ll> ps;
        for (ll d = 2; d * d <= x; d++)
            if (x % d == 0) {
                ps.push_back(d);
                while (x % d == 0) x /= d;
            }
        if (x > 1) ps.push_back(x);
        return ps;
    };
    auto power = [m](ll b, ll e) {
        ll r = 1 % m;
        for (b %= m; e; e >>= 1, b = b * b % m)
            if (e & 1) r = r * b % m;
        return r;
    };
    if (m <= 4) return m == 1 ? 0 : m - 1;  // 1 -> 0, 2 -> 1, 3 -> 2, 4 -> 3
    vector<ll> pm = primes_of(m);
    ll odd = m % 2 == 0 ? m / 2 : m;  // a root exists iff m = p^k or 2 p^k with p odd
    if (odd % 2 == 0 || primes_of(odd).size() != 1) return -1;
    ll phi = m;
    for (ll p : pm) phi = phi / p * (p - 1);
    vector<ll> fs = primes_of(phi);
    for (ll g = 2; g < m; g++) {
        if (gcd(g, m) != 1) continue;
        bool ok = true;
        for (ll q : fs)
            if (power(g, phi / q) == 1) { ok = false; break; }  // order of g divides phi / q
        if (ok) return g;
    }
    return -1;
}
// @end
