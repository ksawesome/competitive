// @name   Digit DP
// @prefix cpdigit
// @key    t =
// @desc   [global] count_upto(N) = how many x in [0, N] have no two equal adjacent digits. Change the state / check for your property.
// @usage  ll ans = count_upto(B) - count_upto(A - 1);  // count in [A, B]
// @complexity O(digits * states * 10)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll count_upto(ll N) {
    if (N < 0) return 0;
    string s = to_string(N);
    int L = s.size();
    // memo[pos][prev digit, 10 = none][tight][started]
    vector<vector<array<array<ll, 2>, 2>>> memo(L, vector<array<array<ll, 2>, 2>>(11, {{{-1, -1}, {-1, -1}}}));
    auto go = [&](auto&& self, int pos, int prev, bool tight, bool started) -> ll {
        if (pos == L) return 1;
        ll& res = memo[pos][prev][tight][started];
        if (res != -1) return res;
        res = 0;
        int lim = tight ? s[pos] - '0' : 9;
        for (int d = 0; d <= lim; d++) {
            if (started && d == prev) continue;  // the property being counted
            bool now = started || d != 0;        // leading zeros don't start the number
            res += self(self, pos + 1, now ? d : 10, tight && d == lim, now);
        }
        return res;
    };
    return go(go, 0, 10, true, false);
}
// @end
