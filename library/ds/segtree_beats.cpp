// @name   Segment tree beats (range chmin, sum, max)
// @prefix cpbeats
// @key    y s
// @desc   [global] Beats st(a): st.chmin(l, r, x) sets a[i] = min(a[i], x) on [l, r); st.sum(l, r), st.max(l, r). Amortized O(log^2 n).
// @usage  Beats st(a);
// @usage  st.chmin(l, r, x); ll s = st.sum(l, r); ll m = st.max(l, r);
// @complexity O((n + q) log^2 n) amortized
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Beats {
    int n;
    vector<ll> mx, smx, cnt, sm;  // max, strict second max, count of max, sum
    template <class U>
    Beats(const vector<U>& a) : n(a.size()), mx(4 * std::max<int>(n, 1)), smx(4 * std::max<int>(n, 1)), cnt(4 * std::max<int>(n, 1)), sm(4 * std::max<int>(n, 1)) {
        if (n) build(1, 0, n, a);
    }
    void chmin(int l, int r, ll v) { chmin(1, 0, n, l, r, v); }
    ll sum(int l, int r) { return sum(1, 0, n, l, r); }
    ll max(int l, int r) { return max(1, 0, n, l, r); }

   private:
    template <class U>
    void build(int x, int lo, int hi, const vector<U>& a) {
        if (hi - lo == 1) {
            mx[x] = sm[x] = a[lo], smx[x] = LLONG_MIN, cnt[x] = 1;
            return;
        }
        int mid = (lo + hi) / 2;
        build(2 * x, lo, mid, a), build(2 * x + 1, mid, hi, a);
        pull(x);
    }
    void pull(int x) {
        int a = 2 * x, b = 2 * x + 1;
        sm[x] = sm[a] + sm[b];
        if (mx[a] == mx[b]) mx[x] = mx[a], cnt[x] = cnt[a] + cnt[b], smx[x] = std::max(smx[a], smx[b]);
        else if (mx[a] > mx[b]) mx[x] = mx[a], cnt[x] = cnt[a], smx[x] = std::max(smx[a], mx[b]);
        else mx[x] = mx[b], cnt[x] = cnt[b], smx[x] = std::max(mx[a], smx[b]);
    }
    void apply(int x, ll v) {  // lower the max of x to v, valid while smx[x] < v
        if (v >= mx[x]) return;
        sm[x] -= (mx[x] - v) * cnt[x], mx[x] = v;
    }
    void push(int x) { apply(2 * x, mx[x]), apply(2 * x + 1, mx[x]); }
    void chmin(int x, int lo, int hi, int l, int r, ll v) {
        if (r <= lo || hi <= l || mx[x] <= v) return;
        if (l <= lo && hi <= r && smx[x] < v) return apply(x, v);
        push(x);
        int mid = (lo + hi) / 2;
        chmin(2 * x, lo, mid, l, r, v), chmin(2 * x + 1, mid, hi, l, r, v);
        pull(x);
    }
    ll sum(int x, int lo, int hi, int l, int r) {
        if (r <= lo || hi <= l) return 0;
        if (l <= lo && hi <= r) return sm[x];
        push(x);
        int mid = (lo + hi) / 2;
        return sum(2 * x, lo, mid, l, r) + sum(2 * x + 1, mid, hi, l, r);
    }
    ll max(int x, int lo, int hi, int l, int r) {
        if (r <= lo || hi <= l) return LLONG_MIN;
        if (l <= lo && hi <= r) return mx[x];
        push(x);
        int mid = (lo + hi) / 2;
        return std::max(max(2 * x, lo, mid, l, r), max(2 * x + 1, mid, hi, l, r));
    }
};
// @end
