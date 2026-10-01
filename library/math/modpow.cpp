// @name   Modular power + inverse
// @prefix cpmodpow
// @key    t m
// @desc   [global] modpow(a, e, m = MOD), modinv(a, m = MOD) for prime m. Uses the template's MOD.
// @usage  modpow(2, n);  // 2^n mod MOD
// @usage  modinv(x);    // x^-1 mod MOD
// @complexity O(log e)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
ll modpow(ll a, ll e, ll m = MOD) {  // m < 3e9 so products fit in ll
    ll r = 1 % m;
    a %= m;
    if (a < 0) a += m;
    for (; e > 0; e >>= 1, a = a * a % m)
        if (e & 1) r = r * a % m;
    return r;
}
ll modinv(ll a, ll m = MOD) { return modpow(a, m - 2, m); }  // m must be prime
// @end
