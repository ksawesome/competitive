// @name   Grid paths DP
// @prefix cpgridpaths
// @key    t shift+z
// @desc   [inside solve] needs n, m, grid. ways[r][c] = right/down paths from (0, 0) to (r, c) avoiding '#', mod MOD.
// @usage  // answer: ways[n - 1][m - 1]; for a max-sum path use max(...) + value instead of the sum
// @complexity O(nm)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

void context(int n, int m, vector<string>& grid) {
    // @begin
    vector<vector<ll>> ways(n, vector<ll>(m));
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++) {
            if (grid[r][c] == '#') continue;
            if (r == 0 && c == 0) ways[r][c] = 1;
            else ways[r][c] = ((r ? ways[r - 1][c] : 0) + (c ? ways[r][c - 1] : 0)) % MOD;
        }
    // @end
}
