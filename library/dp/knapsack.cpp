// @name   Knapsack (0/1, unbounded, coin-change ways)
// @prefix cpknap
// @key    t shift+q
// @desc   [global] knapsack_01(items, W) / knapsack_unbounded(items, W): max value, weight <= W; coin_ways(coins, X): combinations mod MOD.
// @usage  vector<pair<int, ll>> items;  // {weight, value}
// @usage  ll best = knapsack_01(items, W); ll ways = coin_ways(coins, X);
// @complexity O(n W)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
template <class I>
ll knapsack_01(const vector<I>& items, int W) {  // each item at most once: capacities go downwards
    vector<ll> dp(W + 1, 0);
    for (auto& [w, v] : items)
        for (int c = W; c >= w; c--) dp[c] = max(dp[c], dp[c - w] + v);
    return dp[W];
}
template <class I>
ll knapsack_unbounded(const vector<I>& items, int W) {  // unlimited copies: capacities go upwards
    vector<ll> dp(W + 1, 0);
    for (auto& [w, v] : items)
        for (int c = w; c <= W; c++) dp[c] = max(dp[c], dp[c - w] + v);
    return dp[W];
}
template <class T>
ll coin_ways(const vector<T>& coins, int X) {  // unordered combinations; for ordered sequences swap the two loops
    vector<ll> dp(X + 1, 0);
    dp[0] = 1;
    for (auto c : coins)
        for (int x = c; x <= X; x++) dp[x] = (dp[x] + dp[x - c]) % MOD;
    return dp[X];
}
// @end
