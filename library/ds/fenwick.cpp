// @name   Fenwick tree / BIT
// @prefix cpbit
// @key    t i
// @desc   [global] BIT b(n); b.add(i, v), b.sum(l, r) on [l, r), b.prefix(i) = sum of [0, i), b.lower_bound(k).
// @usage  BIT b(n);
// @usage  b.add(i, v); b.sum(l, r); b.prefix(i); b.lower_bound(k);
// @complexity O(log n) per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct BIT {
    int n;
    vector<ll> t;
    BIT(int n_) : n(n_), t(n_ + 1) {}
    void add(int i, ll v) {  // 0-indexed
        for (i++; i <= n; i += i & -i) t[i] += v;
    }
    ll prefix(int i) const {  // sum of [0, i)
        ll s = 0;
        for (; i > 0; i -= i & -i) s += t[i];
        return s;
    }
    ll sum(int l, int r) const { return prefix(r) - prefix(l); }  // [l, r)
    int lower_bound(ll k) const {  // first i with sum of [0, i] >= k (values must be >= 0); n if none
        int pos = 0, pw = 1;
        while (pw * 2 <= n) pw *= 2;
        for (; pw; pw >>= 1) {
            if (pos + pw <= n && t[pos + pw] < k) {
                pos += pw;
                k -= t[pos];
            }
        }
        return pos;
    }
};
// @end
