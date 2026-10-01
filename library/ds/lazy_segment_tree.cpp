// @name   Lazy segment tree (range add; range sum + min)
// @prefix cplazy
// @key    t shift+s
// @desc   [global] LazySeg st(n) or st(a); st.add(l, r, v), st.sum_of(l, r), st.min_of(l, r), all on [l, r).
// @usage  LazySeg st(a);
// @usage  st.add(l, r, v); st.sum_of(l, r); st.min_of(l, r);  // all [l, r)
// @complexity O(log n) per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct LazySeg {
    int n;
    vector<ll> sum, mn, lz;
    LazySeg(int n_) : n(n_), sum(4 * n_), mn(4 * n_), lz(4 * n_) {}
    template <class U>
    LazySeg(const vector<U>& a) : LazySeg((int)a.size()) {
        if (n) build(1, 0, n, a);
    }
    void add(int l, int r, ll v) { add(1, 0, n, l, r, v); }  // a[i] += v for i in [l, r)
    ll sum_of(int l, int r) { return query(1, 0, n, l, r).first; }
    ll min_of(int l, int r) { return query(1, 0, n, l, r).second; }

   private:
    template <class U>
    void build(int x, int lo, int hi, const vector<U>& a) {
        if (hi - lo == 1) {
            sum[x] = mn[x] = a[lo];
            return;
        }
        int mid = (lo + hi) / 2;
        build(2 * x, lo, mid, a);
        build(2 * x + 1, mid, hi, a);
        pull(x);
    }
    void apply(int x, int lo, int hi, ll v) {
        sum[x] += v * (hi - lo);
        mn[x] += v;
        lz[x] += v;
    }
    void push(int x, int lo, int hi) {
        if (!lz[x]) return;
        int mid = (lo + hi) / 2;
        apply(2 * x, lo, mid, lz[x]);
        apply(2 * x + 1, mid, hi, lz[x]);
        lz[x] = 0;
    }
    void pull(int x) {
        sum[x] = sum[2 * x] + sum[2 * x + 1];
        mn[x] = min(mn[2 * x], mn[2 * x + 1]);
    }
    void add(int x, int lo, int hi, int l, int r, ll v) {
        if (r <= lo || hi <= l) return;
        if (l <= lo && hi <= r) return apply(x, lo, hi, v);
        push(x, lo, hi);
        int mid = (lo + hi) / 2;
        add(2 * x, lo, mid, l, r, v);
        add(2 * x + 1, mid, hi, l, r, v);
        pull(x);
    }
    pair<ll, ll> query(int x, int lo, int hi, int l, int r) {
        if (r <= lo || hi <= l) return {0, LLONG_MAX};
        if (l <= lo && hi <= r) return {sum[x], mn[x]};
        push(x, lo, hi);
        int mid = (lo + hi) / 2;
        auto a = query(2 * x, lo, mid, l, r), b = query(2 * x + 1, mid, hi, l, r);
        return {a.first + b.first, min(a.second, b.second)};
    }
};
// @end
