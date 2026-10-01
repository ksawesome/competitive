// @name   Extended GCD + inverse for any modulus
// @prefix cpextgcd
// @key    t x
// @desc   [global] g = ext_gcd(a, b, x, y) with a*x + b*y = g; inv_mod(a, m) for gcd(a, m) = 1, else -1.
// @usage  ll x, y, g = ext_gcd(a, b, x, y);  // a*x + b*y = g
// @usage  inv_mod(a, m);  // -1 if gcd(a, m) != 1
// @complexity O(log min(a, b))
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll ext_gcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll x1, y1, g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
ll inv_mod(ll a, ll m) {
    ll x, y;
    if (ext_gcd((a % m + m) % m, m, x, y) != 1) return -1;
    return (x % m + m) % m;
}
// @end
