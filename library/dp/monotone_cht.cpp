// @name   Monotone convex hull trick (sorted slopes)
// @prefix cpmonocht
// @key    y z
// @desc   [global] MonoCHT h; h.add(k, m) with non-increasing k; h.query(x) = min of k*x + m (binary search); h.query_inc(x) for non-decreasing x (pointer walk). For max: add(-k, -m), negate.
// @usage  MonoCHT h; h.add(b[j], dp[j]);
// @usage  dp[i] = h.query_inc(a[i]) + c[i];
// @complexity O(1) amortized add, O(log n) query, O(1) amortized query_inc
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct MonoCHT {
    struct Ln {
        ll k, m;
        ll at(ll x) const { return k * x + m; }
    };
    vector<Ln> h;  // slopes strictly decreasing; h[i] is the minimum on an interval of x left of h[i+1]'s
    size_t ptr = 0;
    static ll fl(ll a, ll b) { return a / b - ((a % b != 0) && ((a < 0) != (b < 0))); }
    static bool frac_le(ll a, ll b, ll c, ll d) {  // a/b <= c/d for b, d > 0, exact (no overflow)
        ll q1 = fl(a, b), q2 = fl(c, d);
        if (q1 != q2) return q1 < q2;
        ll r1 = a - q1 * b, r2 = c - q2 * d;
        if (r1 == 0) return true;
        if (r2 == 0) return false;
        return frac_le(d, r2, b, r1);
    }
    // b is useless if the point where c beats b is not right of the point where b beats a
    static bool useless(const Ln& a, const Ln& b, const Ln& c) {
        return frac_le(c.m - b.m, b.k - c.k, b.m - a.m, a.k - b.k);
    }
    void add(ll k, ll m) {
        Ln nw{k, m};
        if (!h.empty() && h.back().k == k) {
            if (h.back().m <= m) return;
            h.pop_back();
        }
        while (h.size() >= 2 && useless(h[h.size() - 2], h.back(), nw)) h.pop_back();
        h.push_back(nw);
        ptr = min(ptr, h.size() - 1);
    }
    ll query(ll x) const {  // any order of x
        size_t lo = 0, hi = h.size() - 1;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (h[mid].at(x) <= h[mid + 1].at(x)) hi = mid;
            else lo = mid + 1;
        }
        return h[lo].at(x);
    }
    ll query_inc(ll x) {  // x never decreases between calls
        while (ptr + 1 < h.size() && h[ptr + 1].at(x) <= h[ptr].at(x)) ptr++;
        return h[ptr].at(x);
    }
};
// @end
