// @name   Range (interval) DP
// @prefix cprangedp
// @key    t -
// @desc   [inside solve] needs n, a. dp[l][r] over intervals by increasing length; example: merge adjacent piles, cost = merged sum.
// @usage  // answer: dp[0][n - 1]; change the inner transition for your problem
// @complexity O(n^3)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n, vector<int>& a) {
    // @begin
    vector<ll> pre(n + 1);
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + a[i];
    vector<vector<ll>> dp(n, vector<ll>(n, 0));  // dp[l][r] = best for a[l..r]
    for (int len = 2; len <= n; len++)
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            dp[l][r] = LLONG_MAX;
            for (int k = l; k < r; k++) dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r] + pre[r + 1] - pre[l]);
        }
    // @end
}
