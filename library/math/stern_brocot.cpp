// @name   Stern-Brocot: simplest fraction in an interval, best approximation
// @prefix cpsternbrocot
// @key    u b
// @desc   [global] simplest_between(a, b, c, d): p/q with the smallest q (then p) and a/b < p/q < c/d (0 <= a/b < c/d); best_approx(x, y, N): p/q closest to x/y with q <= N.
// @usage  auto [p, q] = simplest_between(1, 3, 1, 2);   // 2/5
// @usage  auto [p2, q2] = best_approx(314159, 100000, 100);   // 311/99
// @complexity O(log max) (a Euclid-like walk down the Stern-Brocot tree)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
pair<ll, ll> simplest_between(ll a, ll b, ll c, ll d) {
    if (a == 0) return {1, d / c + 1};                    // (0, c/d): 1/q with q > d/c
    if (a >= b) {                                         // shift both by the integer part k = floor(a/b)
        ll k = a / b;
        auto [p, q] = simplest_between(a - k * b, b, c - k * d, d);
        return {p + k * q, q};
    }
    if (c > d) return {1, 1};                             // a/b < 1 < c/d
    auto [q, p] = simplest_between(d, c, b, a);           // both in (0, 1]: flip to d/c < q/p < b/a
    return {p, q};
}
pair<ll, ll> best_approx(ll x, ll y, ll N) {  // x, y > 0, N >= 1; the error is compared in long double
    ll p0 = 0, q0 = 1, p1 = 1, q1 = 0, a = x, b = y;
    while (b) {
        ll t = a / b;
        if (q0 + t * q1 > N) {  // the next convergent is too big: try the best semiconvergent too
            ll k = (N - q0) / q1, ps = p0 + k * p1, qs = q0 + k * q1;
            long double e1 = fabsl((long double)x / y - (long double)p1 / q1), e2 = fabsl((long double)x / y - (long double)ps / qs);
            return e2 < e1 ? make_pair(ps, qs) : make_pair(p1, q1);
        }
        ll p2 = p0 + t * p1, q2 = q0 + t * q1;
        p0 = p1, q0 = q1, p1 = p2, q1 = q2;
        ll r = a - t * b;
        a = b, b = r;
    }
    return {p1, q1};
}
// @end
