// @name   (min, +) convolution with convex sequences
// @prefix cpminplus
// @key    u shift+c
// @desc   [global] c[k] = min over i + j = k of a[i] + b[j]. minplus_convex_convex(a, b): both convex, O(n + m) by merging slopes; minplus_convex_any(a, b): a convex, b arbitrary, O((n + m) log).
// @usage  vector<ll> c = minplus_convex_convex(a, b);
// @usage  vector<ll> d = minplus_convex_any(convex_a, arbitrary_b);
// @complexity O(n + m) / O((n + m) log(n + m))
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<ll> minplus_convex_convex(const vector<ll>& a, const vector<ll>& b) {
    // the result is convex; its slopes are the slopes of a and b merged in sorted order (Minkowski sum)
    int n = a.size(), m = b.size();
    vector<ll> c(n + m - 1);
    c[0] = a[0] + b[0];
    for (int k = 1, i = 0, j = 0; k < n + m - 1; k++) {
        if (j == m - 1 || (i < n - 1 && a[i + 1] - a[i] <= b[j + 1] - b[j])) c[k] = c[k - 1] + a[i + 1] - a[i], i++;
        else c[k] = c[k - 1] + b[j + 1] - b[j], j++;
    }
    return c;
}
vector<ll> minplus_convex_any(const vector<ll>& a, const vector<ll>& b) {
    // for fixed k the best j (b's index) moves right as k grows, because a is convex; divide and conquer on k
    int n = a.size(), m = b.size(), K = n + m - 1;
    vector<ll> c(K);
    auto solve = [&](auto&& self, int k1, int k2, int j1, int j2) -> void {
        if (k1 >= k2) return;
        int k = (k1 + k2) / 2, lo = max(j1, k - n + 1), hi = min(j2, k), best = lo;
        ll bv = LLONG_MAX;
        for (int j = lo; j <= hi; j++)
            if (b[j] + a[k - j] < bv) bv = b[j] + a[k - j], best = j;
        c[k] = bv;
        self(self, k1, k, j1, best);
        self(self, k + 1, k2, best, j2);
    };
    solve(solve, 0, K, 0, m - 1);
    return c;
}
// @end
