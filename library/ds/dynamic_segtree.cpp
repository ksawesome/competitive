// @name   Dynamic (sparse) segment tree
// @prefix cpdynseg
// @key    y d
// @desc   [global] DynSeg st(lo, hi): positions in [lo, hi) as large as 1e18, nodes made on demand; st.add(pos, v), st.sum(l, r) on [l, r).
// @usage  DynSeg st(0, 1e9);
// @usage  st.add(x, 1); ll c = st.sum(l, r);
// @complexity O(log range) per operation, O(q log range) memory
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct DynSeg {
    struct Nd {
        ll sum = 0;
        int l = 0, r = 0;  // child indices, 0 = none
    };
    vector<Nd> t;
    ll lo, hi;
    DynSeg(ll lo_, ll hi_) : t(2), lo(lo_), hi(hi_) {}  // t[1] is the root, t[0] a null node
    void add(ll pos, ll v) {
        int x = 1;
        for (ll a = lo, b = hi;;) {
            t[x].sum += v;
            if (b - a == 1) return;
            ll mid = a + (b - a) / 2;
            bool right = pos >= mid;
            int child = right ? t[x].r : t[x].l;
            if (!child) {
                child = t.size();
                t.emplace_back();
                (right ? t[x].r : t[x].l) = child;
            }
            x = child;
            if (right) a = mid;
            else b = mid;
        }
    }
    ll sum(ll l, ll r) const { return sum(1, lo, hi, l, r); }

   private:
    ll sum(int x, ll a, ll b, ll l, ll r) const {
        if (!x || r <= a || b <= l) return 0;
        if (l <= a && b <= r) return t[x].sum;
        ll mid = a + (b - a) / 2;
        return sum(t[x].l, a, mid, l, r) + sum(t[x].r, mid, b, l, r);
    }
};
// @end
