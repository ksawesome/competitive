// @name   Chinese remainder theorem (any moduli)
// @prefix cpcrt
// @key    y 1
// @desc   [global] crt(r1, m1, r2, m2) -> {r, lcm}: x = r (mod lcm) solves both congruences; {-1, -1} if none. Moduli need not be coprime.
// @usage  auto [r, m] = crt(2, 3, 3, 5);  // x = 8 (mod 15)
// @usage  // many congruences: fold left, feeding each result back in as (r1, m1); lcm must stay < ~4e18
// @complexity O(log m)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
pair<ll, ll> crt(ll r1, ll m1, ll r2, ll m2) {
    auto mulmod = [](ll a, ll b, ll m) {  // a * b mod m for 0 <= a, b < m, via long double (no __int128)
        using ull = unsigned long long;
        ll r = (ll)((ull)a * (ull)b - (ull)m * (ull)((long double)a * b / m));
        return r < 0 ? r + m : (r >= m ? r - m : r);
    };
    auto egcd = [](auto&& self, ll a, ll b, ll& x, ll& y) -> ll {
        if (!b) return x = 1, y = 0, a;
        ll g = self(self, b, a % b, y, x);
        y -= a / b * x;
        return g;
    };
    r1 = (r1 % m1 + m1) % m1, r2 = (r2 % m2 + m2) % m2;
    ll x, y, g = egcd(egcd, m1, m2, x, y);  // m1 x + m2 y = g
    if ((r2 - r1) % g) return {-1, -1};
    ll mod = m2 / g, l = m1 / g * m2;
    ll t = mulmod(((r2 - r1) / g % mod + mod) % mod, (x % mod + mod) % mod, mod);  // m1 t = r2 - r1 (mod m2)
    return {(r1 + mulmod(m1 % l, t, l)) % l, l};
}
// @end
