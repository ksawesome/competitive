// @name   Fenwick tree: range add + range sum
// @prefix cpbitrange
// @key    y i
// @desc   [global] BITRange b(n); b.add(l, r, v) adds v on [l, r); b.sum(l, r). Two Fenwick trees over the difference array.
// @usage  BITRange b(n);
// @usage  b.add(l, r, v); ll s = b.sum(l, r);
// @complexity O(log n) per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct BITRange {
    int n;
    vector<ll> b1, b2;  // b1: difference d[i], b2: i * d[i]
    BITRange(int n_) : n(n_), b1(n_ + 1), b2(n_ + 1) {}
    void upd(vector<ll>& b, int i, ll v) {
        for (i++; i <= n; i += i & -i) b[i] += v;
    }
    ll qry(const vector<ll>& b, int i) const {  // sum over [0, i)
        ll s = 0;
        for (; i > 0; i -= i & -i) s += b[i];
        return s;
    }
    void add(int l, int r, ll v) { upd(b1, l, v), upd(b1, r, -v), upd(b2, l, v * l), upd(b2, r, -v * r); }
    ll prefix(int i) const { return qry(b1, i) * i - qry(b2, i); }  // sum of [0, i)
    ll sum(int l, int r) const { return prefix(r) - prefix(l); }
};
// @end
