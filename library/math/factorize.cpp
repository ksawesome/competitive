// @name   Factorize + divisors (trial division)
// @prefix cpfactor
// @key    t shift+p
// @desc   [global] factorize(x) -> {prime, exponent}, divisors(x) sorted. O(sqrt x), fine up to ~1e12.
// @usage  for (auto [p, e] : factorize(x)) { ... }
// @usage  vector<ll> d = divisors(x);
// @complexity O(sqrt x)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<pair<ll, int>> factorize(ll x) {
    vector<pair<ll, int>> f;
    for (ll p = 2; p * p <= x; p++) {
        if (x % p) continue;
        int e = 0;
        while (x % p == 0) x /= p, e++;
        f.push_back({p, e});
    }
    if (x > 1) f.push_back({x, 1});
    return f;
}
vector<ll> divisors(ll x) {
    vector<ll> d;
    for (ll i = 1; i * i <= x; i++) {
        if (x % i) continue;
        d.push_back(i);
        if (i != x / i) d.push_back(x / i);
    }
    sort(d.begin(), d.end());
    return d;
}
// @end
