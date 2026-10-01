// @name   Subset sums with bitset
// @prefix cpbitsetsum
// @key    t shift+1
// @desc   [global] subset_sums<MAXS>(a): bitset with bit s set iff some subset of a sums to s (s < MAXS). 64x faster than a bool DP.
// @usage  auto can = subset_sums<100001>(a);
// @usage  if (can[target]) { ... }
// @complexity O(n MAXS / 64)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <size_t MAXS, class T>
bitset<MAXS> subset_sums(const vector<T>& a) {
    bitset<MAXS> b;
    b[0] = 1;
    for (auto x : a) b |= b << x;
    return b;
}
// @end
