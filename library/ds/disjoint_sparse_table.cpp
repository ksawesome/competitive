// @name   Disjoint sparse table (O(1) queries for any associative operation)
// @prefix cpdisjointsparse
// @key    u t
// @desc   [global] DisjointSparse<T, Op> t(a, op); t.query(l, r) = a[l] op ... op a[r-1] for [l, r), l < r. Works for sums, products mod p, matrix products: no need for idempotence.
// @usage  DisjointSparse t(a, [](ll x, ll y) { return x * y % MOD; });
// @usage  ll prod = t.query(l, r);
// @complexity O(n log n) build, O(1) query
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class T, class Op>
struct DisjointSparse {
    vector<vector<T>> t;  // level h: blocks of size 2^h; inside a block, suffixes of the left half and prefixes of the right
    vector<T> a;
    Op op;
    int sz2 = 1;
    DisjointSparse(const vector<T>& v, Op f) : a(v), op(f) {
        int n = v.size(), lg = 1;
        while (sz2 < n) sz2 <<= 1, lg++;
        t.assign(lg, vector<T>(n));
        for (int h = 1; h < lg; h++) {
            int half = 1 << (h - 1);
            for (int mid = half; mid < n; mid += 2 * half) {
                t[h][mid - 1] = a[mid - 1];
                for (int i = mid - 2; i >= mid - half; i--) t[h][i] = op(a[i], t[h][i + 1]);
                t[h][mid] = a[mid];
                for (int i = mid + 1; i < min(n, mid + half); i++) t[h][i] = op(t[h][i - 1], a[i]);
            }
        }
    }
    T query(int l, int r) const {  // [l, r)
        r--;
        if (l == r) return a[l];
        int h = 32 - __builtin_clz((unsigned)(l ^ r));  // highest differing bit + 1 = level whose midpoint splits l and r
        return op(t[h][l], t[h][r]);
    }
};
// @end
