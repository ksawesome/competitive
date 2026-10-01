// @name   Fenwick tree 2D
// @prefix cpbit2d
// @key    y shift+i
// @desc   [global] BIT2D b(n, m); b.add(r, c, v); b.sum(r1, c1, r2, c2) = sum over rows [r1, r2) x cols [c1, c2).
// @usage  BIT2D b(n, m);
// @usage  b.add(r, c, 1); ll s = b.sum(r1, c1, r2, c2);
// @complexity O(log n log m) per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct BIT2D {
    int n, m;
    vector<vector<ll>> t;
    BIT2D(int n_, int m_) : n(n_), m(m_), t(n_ + 1, vector<ll>(m_ + 1)) {}
    void add(int r, int c, ll v) {
        for (int i = r + 1; i <= n; i += i & -i)
            for (int j = c + 1; j <= m; j += j & -j) t[i][j] += v;
    }
    ll prefix(int r, int c) const {  // sum over [0, r) x [0, c)
        ll s = 0;
        for (int i = r; i > 0; i -= i & -i)
            for (int j = c; j > 0; j -= j & -j) s += t[i][j];
        return s;
    }
    ll sum(int r1, int c1, int r2, int c2) const {
        return prefix(r2, c2) - prefix(r1, c2) - prefix(r2, c1) + prefix(r1, c1);
    }
};
// @end
