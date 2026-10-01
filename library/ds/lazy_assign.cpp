// @name   Lazy segment tree (range assign + range add; sum, min)
// @prefix cplazyassign
// @key    y a
// @desc   [global] LazyAssign st(n or a): st.assign(l, r, v), st.add(l, r, v), st.sum_of(l, r), st.min_of(l, r), all on [l, r).
// @usage  LazyAssign st(a);
// @usage  st.assign(l, r, 5); st.add(l, r, -2); st.sum_of(l, r); st.min_of(l, r);
// @complexity O(log n) per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct LazyAssign {
    int n;
    vector<ll> sum, mn, add_tag, set_val;
    vector<bool> has_set;  // pending "assign set_val" (applied before add_tag, which is then 0)
    LazyAssign(int n_) : n(n_), sum(4 * n_), mn(4 * n_), add_tag(4 * n_), set_val(4 * n_), has_set(4 * n_) {}
    template <class U>
    LazyAssign(const vector<U>& a) : LazyAssign((int)a.size()) {
        if (n) build(1, 0, n, a);
    }
    void assign(int l, int r, ll v) { update(1, 0, n, l, r, v, true); }
    void add(int l, int r, ll v) { update(1, 0, n, l, r, v, false); }
    ll sum_of(int l, int r) { return query(1, 0, n, l, r).first; }
    ll min_of(int l, int r) { return query(1, 0, n, l, r).second; }

   private:
    template <class U>
    void build(int x, int lo, int hi, const vector<U>& a) {
        if (hi - lo == 1) return void(sum[x] = mn[x] = a[lo]);
        int mid = (lo + hi) / 2;
        build(2 * x, lo, mid, a), build(2 * x + 1, mid, hi, a);
        pull(x);
    }
    void apply(int x, int len, ll v, bool is_set) {
        if (is_set) sum[x] = v * len, mn[x] = v, has_set[x] = true, set_val[x] = v, add_tag[x] = 0;
        else {
            sum[x] += v * len, mn[x] += v;
            if (has_set[x]) set_val[x] += v;
            else add_tag[x] += v;
        }
    }
    void push(int x, int lo, int hi) {
        int mid = (lo + hi) / 2;
        if (has_set[x]) {
            apply(2 * x, mid - lo, set_val[x], true), apply(2 * x + 1, hi - mid, set_val[x], true);
            has_set[x] = false;
        }
        if (add_tag[x]) {
            apply(2 * x, mid - lo, add_tag[x], false), apply(2 * x + 1, hi - mid, add_tag[x], false);
            add_tag[x] = 0;
        }
    }
    void pull(int x) { sum[x] = sum[2 * x] + sum[2 * x + 1], mn[x] = min(mn[2 * x], mn[2 * x + 1]); }
    void update(int x, int lo, int hi, int l, int r, ll v, bool is_set) {
        if (r <= lo || hi <= l) return;
        if (l <= lo && hi <= r) return apply(x, hi - lo, v, is_set);
        push(x, lo, hi);
        int mid = (lo + hi) / 2;
        update(2 * x, lo, mid, l, r, v, is_set), update(2 * x + 1, mid, hi, l, r, v, is_set);
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
