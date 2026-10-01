// @name   Floor-division blocks
// @prefix cpfloorblocks
// @key    y shift+f
// @desc   [global] for_each_floor_block(n, f): f(l, r, q) for every maximal [l, r] where n / i == q; only O(sqrt n) blocks.
// @usage  ll s = 0; for_each_floor_block(n, [&](ll l, ll r, ll q) { s += q * (r - l + 1); });  // sum of n / i
// @usage  // sum of divisors of 1..n: q * (l + r) * (r - l + 1) / 2 per block
// @complexity O(sqrt n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class F>
void for_each_floor_block(ll n, F f) {
    for (ll l = 1, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        f(l, r, q);
    }
}
// @end
