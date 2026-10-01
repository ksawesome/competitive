// @name   Stirling numbers (both kinds) and Bell numbers
// @prefix cpstirling
// @key    u s
// @desc   [global] init_stirling(N): S2[n][k] (partitions of n items into k nonempty sets), S1[n][k] (permutations of n with k cycles), bell[n]; all mod MOD.
// @usage  init_stirling(1000);
// @usage  S2[n][k]; S1[n][k]; bell[n];
// @complexity O(N^2) time and memory
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
vector<vector<ll>> S1, S2;  // unsigned Stirling numbers of the first kind, Stirling numbers of the second kind
vector<ll> bell;
void init_stirling(int N) {
    S1.assign(N + 1, vector<ll>(N + 1, 0)), S2 = S1;
    bell.assign(N + 1, 0);
    S1[0][0] = S2[0][0] = 1;
    for (int n = 1; n <= N; n++)
        for (int k = 1; k <= n; k++) {
            S1[n][k] = (S1[n - 1][k - 1] + (ll)(n - 1) * S1[n - 1][k]) % MOD;  // n alone in a cycle, or inserted after one of n-1
            S2[n][k] = (S2[n - 1][k - 1] + (ll)k * S2[n - 1][k]) % MOD;        // n alone in a set, or joins one of k sets
        }
    for (int n = 0; n <= N; n++)
        for (int k = 0; k <= n; k++) bell[n] = (bell[n] + S2[n][k]) % MOD;
}
// @end
