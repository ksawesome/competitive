// @name   Difference array (offline range add, 1D)
// @prefix cpdiff
// @key    t 3
// @desc   [inside solve] needs n. range_add(l, r, v) adds v to a[l, r) in O(1); build() returns the final array.
// @usage  range_add(l, r, v);            // any number of times
// @usage  vector<ll> a = build();        // once, at the end
// @complexity O(1) per update, O(n) build
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n) {
    // @begin
    vector<ll> diff(n + 1);
    auto range_add = [&](int l, int r, ll v) { diff[l] += v, diff[r] -= v; };  // [l, r)
    auto build = [&]() {
        vector<ll> res(n);
        ll run = 0;
        for (int i = 0; i < n; i++) res[i] = run += diff[i];
        return res;
    };
    // @end
    (void)range_add, (void)build;
}
