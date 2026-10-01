// @name   Difference array (offline rectangle add, 2D)
// @prefix cpdiff2d
// @key    t 4
// @desc   [inside solve] needs n, m. rect_add(r1, c1, r2, c2, v) adds v to rows [r1, r2) x cols [c1, c2); build() returns the grid.
// @usage  rect_add(r1, c1, r2, c2, v);   // any number of times
// @usage  auto g = build();              // g[r][c] = total added to cell (r, c)
// @complexity O(1) per update, O(nm) build
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n, int m) {
    // @begin
    vector<vector<ll>> D(n + 1, vector<ll>(m + 1));
    auto rect_add = [&](int r1, int c1, int r2, int c2, ll v) {
        D[r1][c1] += v, D[r1][c2] -= v, D[r2][c1] -= v, D[r2][c2] += v;
    };
    auto build = [&]() {
        vector<vector<ll>> g(n, vector<ll>(m));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                g[i][j] = D[i][j] + (i ? g[i - 1][j] : 0) + (j ? g[i][j - 1] : 0) - (i && j ? g[i - 1][j - 1] : 0);
        return g;
    };
    // @end
    (void)rect_add, (void)build;
}
