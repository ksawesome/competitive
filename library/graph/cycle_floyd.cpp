// @name   Cycle finding in O(1) memory (Floyd / Brent)
// @prefix cpcyclefind
// @key    u f
// @desc   [global] find_rho(x0, f): for the sequence x0, f(x0), f(f(x0)), ... returns {mu, lambda}: the first index of the cycle and its length. Brent's variant: O(mu + lambda) calls of f, O(1) memory (use when states are huge).
// @usage  auto [mu, lam] = find_rho<ll>(seed, [](ll x) { return (x * x + 1) % 1'000'003; });
// @complexity O(mu + lambda) evaluations of f, O(1) memory
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class T, class F>
pair<ll, ll> find_rho(T x0, F f) {
    // Brent: the hare jumps ahead in powers of two; when it meets the tortoise, lambda is known
    ll power = 1, lam = 1;
    T tortoise = x0, hare = f(x0);
    while (!(tortoise == hare)) {
        if (power == lam) tortoise = hare, power *= 2, lam = 0;
        hare = f(hare), lam++;
    }
    // mu: start one pointer lambda steps ahead, move both until they meet at the cycle entry
    T a = x0, b = x0;
    for (ll i = 0; i < lam; i++) b = f(b);
    ll mu = 0;
    while (!(a == b)) a = f(a), b = f(b), mu++;
    return {mu, lam};
}
// @end
