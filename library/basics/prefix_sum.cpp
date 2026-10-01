// @name   Prefix sums (1D)
// @prefix cpprefix
// @key    t 1
// @desc   [inside solve] needs n, a. P[i] = a[0] + ... + a[i-1]; range_sum(l, r) = a[l] + ... + a[r-1].
// @usage  ll s = range_sum(l, r);  // [l, r)
// @usage  // subarrays with sum k: count pairs i < j with P[j] - P[i] == k using a map of seen P values
// @complexity O(n) build, O(1) per query
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n, vector<int>& a) {
    // @begin
    vector<ll> P(n + 1);
    for (int i = 0; i < n; i++) P[i + 1] = P[i] + a[i];
    auto range_sum = [&](int l, int r) { return P[r] - P[l]; };  // [l, r)
    // @end
    (void)range_sum;
}
