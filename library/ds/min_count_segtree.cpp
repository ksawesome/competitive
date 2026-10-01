// @name   Segment tree: range add, min + count of min
// @prefix cpmincount
// @key    y shift+c
// @desc   [global] MinCount st(weights): add(l, r, v); query(l, r) = {min, total weight at the min}; zero_weight() = weight of cells equal to 0 (values must stay >= 0, as coverage counts do).
// @usage  MinCount st(len);  // len[i] = width of elementary interval i (all 1 for plain counting)
// @usage  st.add(l, r, +1); st.add(l, r, -1); ll covered = total_width - st.zero_weight();  // union of rectangles sweep
// @complexity O(log n) per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct MinCount {
    int n;
    vector<ll> mn, cnt, lz;
    template <class U>
    MinCount(const vector<U>& weight) : n(weight.size()), mn(4 * max<int>(n, 1)), cnt(4 * max<int>(n, 1)), lz(4 * max<int>(n, 1)) {
        if (n) build(1, 0, n, weight);
    }
    void add(int l, int r, ll v) { add(1, 0, n, l, r, v); }
    pair<ll, ll> query(int l, int r) { return query(1, 0, n, l, r); }
    ll zero_weight() { return mn[1] == 0 ? cnt[1] : 0; }

   private:
    template <class U>
    void build(int x, int lo, int hi, const vector<U>& w) {
        if (hi - lo == 1) return void(cnt[x] = w[lo]);
        int mid = (lo + hi) / 2;
        build(2 * x, lo, mid, w), build(2 * x + 1, mid, hi, w);
        cnt[x] = cnt[2 * x] + cnt[2 * x + 1];
    }
    void pull(int x) {
        mn[x] = min(mn[2 * x], mn[2 * x + 1]);
        cnt[x] = (mn[2 * x] == mn[x] ? cnt[2 * x] : 0) + (mn[2 * x + 1] == mn[x] ? cnt[2 * x + 1] : 0);
    }
    void apply(int x, ll v) { mn[x] += v, lz[x] += v; }
    void add(int x, int lo, int hi, int l, int r, ll v) {
        if (r <= lo || hi <= l) return;
        if (l <= lo && hi <= r) return apply(x, v);
        if (lz[x]) apply(2 * x, lz[x]), apply(2 * x + 1, lz[x]), lz[x] = 0;
        int mid = (lo + hi) / 2;
        add(2 * x, lo, mid, l, r, v), add(2 * x + 1, mid, hi, l, r, v);
        pull(x);
    }
    pair<ll, ll> query(int x, int lo, int hi, int l, int r) {
        if (r <= lo || hi <= l) return {LLONG_MAX, 0};
        if (l <= lo && hi <= r) return {mn[x], cnt[x]};
        if (lz[x]) apply(2 * x, lz[x]), apply(2 * x + 1, lz[x]), lz[x] = 0;
        int mid = (lo + hi) / 2;
        auto a = query(2 * x, lo, mid, l, r), b = query(2 * x + 1, mid, hi, l, r);
        ll m = min(a.first, b.first);
        return {m, (a.first == m ? a.second : 0) + (b.first == m ? b.second : 0)};
    }
};
// @end
