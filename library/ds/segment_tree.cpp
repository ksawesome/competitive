// @name   Segment tree (point update, range query)
// @prefix cpseg
// @key    t s
// @desc   [global] SegTree st(n) or st(a); st.set(i, x), st.get(i), st.query(l, r) on [l, r). Edit T / ID / op.
// @usage  SegTree st(a);  // or SegTree st(n)
// @usage  st.set(i, x); st.get(i); st.query(l, r);  // [l, r)
// @complexity O(log n) per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct SegTree {
    using T = ll;
    static constexpr T ID = 0;               // identity: 0 for +, LLONG_MAX for min, LLONG_MIN for max
    static T op(T a, T b) { return a + b; }  // any associative op; argument order is preserved
    int n;
    vector<T> t;
    SegTree(int n_) : n(n_), t(2 * n_, ID) {}
    template <class U>
    SegTree(const vector<U>& a) : SegTree((int)a.size()) {
        for (int i = 0; i < n; i++) t[n + i] = a[i];
        for (int i = n - 1; i > 0; i--) t[i] = op(t[2 * i], t[2 * i + 1]);
    }
    void set(int i, T v) {
        for (t[i += n] = v; i > 1; i >>= 1) t[i >> 1] = op(t[i & ~1], t[i | 1]);
    }
    T get(int i) const { return t[i + n]; }
    T query(int l, int r) const {  // [l, r)
        T left = ID, right = ID;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) left = op(left, t[l++]);
            if (r & 1) right = op(t[--r], right);
        }
        return op(left, right);
    }
};
// @end
