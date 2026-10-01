// @name   LineContainer (convex hull trick, any order)
// @prefix cplinecontainer
// @key    y l
// @desc   [global] LineContainer lc; lc.add(k, m); lc.query(x) = max of k*x + m. For a minimum add(-k, -m) and negate the query.
// @usage  LineContainer lc;
// @usage  lc.add(slope, intercept); ll best = lc.query(x);  // dp[i] = max_j (dp[j] + b[j] * a[i]) in O(log n)
// @complexity O(log n) per add / query
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct CHLine {
    mutable ll k, m, p;  // slope, intercept, x where the next line takes over
    bool operator<(const CHLine& o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};
struct LineContainer : multiset<CHLine, less<>> {
    static constexpr ll inf = LLONG_MAX;
    static ll floor_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }
    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, false;
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = floor_div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m) {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }
    ll query(ll x) const {
        auto l = *lower_bound(x);
        return l.k * x + l.m;
    }
};
// @end
