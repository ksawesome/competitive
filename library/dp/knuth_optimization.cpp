// @name   Knuth optimization (interval DP)
// @prefix cpknuth
// @key    y shift+q
// @desc   [global] knuth_opt(n, cost): dp[l][r] = min over l < m < r of dp[l][m] + dp[m][r] + cost(l, r), dp[i][i+1] = 0. Needs monotone opt.
// @usage  ll best = knuth_opt(n, [&](int l, int r) { return pre[r] - pre[l]; });  // optimal merging of a[0, n)
// @complexity O(n^2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class C>
ll knuth_opt(int n, C cost) {
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, 0));
    vector<vector<int>> opt(n + 1, vector<int>(n + 1, 0));
    for (int i = 0; i < n; i++) opt[i][i + 1] = i + 1;
    for (int len = 2; len <= n; len++)
        for (int l = 0; l + len <= n; l++) {
            int r = l + len;
            dp[l][r] = LLONG_MAX;
            for (int m = max(l + 1, opt[l][r - 1]); m <= min(r - 1, opt[l + 1][r]); m++) {
                ll v = dp[l][m] + dp[m][r] + cost(l, r);
                if (v < dp[l][r]) dp[l][r] = v, opt[l][r] = m;
            }
        }
    return dp[0][n];
}
// @end
