// @name   Random generator
// @prefix cprng
// @key    t shift+r
// @desc   [global] rnd(l, r) uniform in [l, r]; shuffle(v.begin(), v.end(), rng). Seeded by time.
// @usage  ll x = rnd(1, 100);
// @usage  shuffle(v.begin(), v.end(), rng);
// @complexity O(1)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
ll rnd(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng); }
// @end
