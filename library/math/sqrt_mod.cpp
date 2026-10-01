// @name   Square root modulo a prime (Tonelli-Shanks)
// @prefix cpsqrtmod
// @key    u q
// @desc   [global] sqrt_mod(a, p): some x with x*x = a (mod p) for prime p (the other root is p - x), or -1 if a is not a square.
// @usage  ll x = sqrt_mod(10, 13);   // 6 or 7: 6*6 = 36 = 10 (mod 13)
// @complexity O(log^2 p)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll sqrt_mod(ll a, ll p) {
    auto power = [p](ll b, ll e) {
        ll r = 1;
        for (b %= p; e; e >>= 1, b = b * b % p)
            if (e & 1) r = r * b % p;
        return r;
    };
    a %= p;
    if (a < 0) a += p;
    if (a == 0 || p == 2) return a;
    if (power(a, (p - 1) / 2) != 1) return -1;  // Euler's criterion: not a quadratic residue
    if (p % 4 == 3) return power(a, (p + 1) / 4);
    ll s = p - 1, e = 0;
    while (s % 2 == 0) s /= 2, e++;  // p - 1 = s * 2^e, s odd
    ll z = 2;
    while (power(z, (p - 1) / 2) != p - 1) z++;  // any non-residue
    ll x = power(a, (s + 1) / 2), t = power(a, s), c = power(z, s), r = e;
    while (t != 1) {  // invariant: x^2 = a * t, t has order 2^i < 2^r
        ll i = 0, tt = t;
        while (tt != 1) tt = tt * tt % p, i++;
        ll bb = power(c, 1LL << (r - i - 1));
        x = x * bb % p, c = bb * bb % p, t = t * c % p, r = i;
    }
    return x;
}
// @end
