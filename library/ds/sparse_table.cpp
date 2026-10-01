// @name   Sparse table (O(1) range min)
// @prefix cpsparse
// @key    t r
// @desc   [global] SparseTable st(a); st.query(l, r) = min of [l, r), l < r. Swap op for max/gcd (idempotent only).
// @usage  SparseTable st(a);
// @usage  st.query(l, r);  // min of [l, r)
// @complexity O(n log n) build, O(1) per query
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
struct SparseTable {
    static T op(T a, T b) { return min(a, b); }
    vector<vector<T>> t;
    SparseTable(const vector<T>& a) : t(1, a) {
        for (int k = 1; (1 << k) <= (int)a.size(); k++) {
            t.emplace_back(a.size() - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= (int)a.size(); i++)
                t[k][i] = op(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
        }
    }
    T query(int l, int r) const {  // [l, r)
        int k = __lg(r - l);
        return op(t[k][l], t[k][r - (1 << k)]);
    }
};
// @end
