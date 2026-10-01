// @name   Bit helpers
// @prefix cpbits
// @key    t y
// @desc   [global] popcnt, highest_bit (floor log2), lowest_bit, has_bit for 64-bit values; submask loop in usage.
// @usage  popcnt(x); highest_bit(x); lowest_bit(x); has_bit(x, i);
// @usage  for (ll s = mask; s; s = (s - 1) & mask) { /* every non-empty submask */ }
// @complexity O(1)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
int popcnt(ll x) { return __builtin_popcountll(x); }
int highest_bit(ll x) { return 63 - __builtin_clzll(x); }  // x > 0
ll lowest_bit(ll x) { return x & -x; }
bool has_bit(ll x, int i) { return x >> i & 1; }
// @end
