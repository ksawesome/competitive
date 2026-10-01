// @name   2D prefix sums
// @prefix cpprefix2d
// @key    t shift+a
// @desc   [inside solve] needs n, m, a (n x m). rect(r1, c1, r2, c2) = sum of rows [r1, r2) x cols [c1, c2).
// @usage  ll s = rect(r1, c1, r2, c2);  // rows [r1, r2), cols [c1, c2)
// @complexity O(nm) build, O(1) per query
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n, int m, vector<vector<int>>& a) {
    // @begin
    vector<vector<ll>> P(n + 1, vector<ll>(m + 1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) P[i + 1][j + 1] = P[i][j + 1] + P[i + 1][j] - P[i][j] + a[i][j];
    auto rect = [&](int r1, int c1, int r2, int c2) { return P[r2][c2] - P[r1][c2] - P[r2][c1] + P[r1][c1]; };
    // @end
    (void)rect;
}
