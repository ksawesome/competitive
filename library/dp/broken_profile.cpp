// @name   Broken profile DP (domino tilings)
// @prefix cpprofile
// @key    y shift+p
// @desc   [global] domino_tilings(n, m): ways to tile an n x m board with 1 x 2 dominoes mod MOD, cell by cell with an m-bit profile (m <= ~12).
// @usage  ll ways = domino_tilings(n, m);  // put the smaller side as m
// @complexity O(n m 2^m)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
ll domino_tilings(int n, int m) {
    vector<ll> dp(1 << m, 0);  // bit j: cell (row, j) is already covered by a vertical domino from above
    dp[0] = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            vector<ll> nd(1 << m, 0);
            for (int mask = 0; mask < (1 << m); mask++) {
                if (!dp[mask]) continue;
                auto put = [&](int to) { nd[to] = (nd[to] + dp[mask]) % MOD; };
                if (mask >> j & 1) put(mask ^ (1 << j));  // already covered: nothing sticks into the next row
                else {
                    put(mask | (1 << j));  // vertical domino down into the next row
                    if (j + 1 < m && !(mask >> (j + 1) & 1)) put(mask | (1 << (j + 1)));  // horizontal to the right
                }
            }
            dp.swap(nd);
        }
    return dp[0];
}
// @end
