// @name   Berlekamp-Massey (find a linear recurrence)
// @prefix cpbm
// @key    u shift+b
// @desc   [global] berlekamp_massey(s): shortest c with s[i] = c[0]*s[i-1] + c[1]*s[i-2] + ... mod MOD (prime). Feed 2L terms to find a recurrence of order L.
// @usage  vector<ll> c = berlekamp_massey({1, 1, 2, 3, 5, 8, 13, 21});   // {1, 1}
// @usage  ll x = linear_rec_nth(first_terms, c, n);   // with the Kitamasa template
// @complexity O(n^2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
vector<ll> berlekamp_massey(const vector<ll>& s) {
    auto power = [](ll b, ll e) {
        ll r = 1;
        for (b %= MOD; e; e >>= 1, b = b * b % MOD)
            if (e & 1) r = r * b % MOD;
        return r;
    };
    vector<ll> cur, last;  // cur = current recurrence; last = the one before the most recent length change
    ll last_fail = -1, last_delta = 0;
    for (size_t i = 0; i < s.size(); i++) {
        ll delta = (s[i] % MOD + MOD) % MOD;  // how far the current recurrence is from predicting s[i]
        for (size_t j = 0; j < cur.size(); j++) delta = (delta - cur[j] * ((s[i - 1 - j] % MOD + MOD) % MOD)) % MOD;
        delta = (delta + MOD) % MOD;
        if (delta == 0) continue;
        if (last_fail < 0) {  // first nonzero term: any recurrence of length i + 1 works
            cur.assign(i + 1, 0), last_fail = i, last_delta = delta;
            continue;
        }
        // cur - (delta / last_delta) * x^(i - last_fail - 1) * (1 - last) fixes position i
        ll coef = delta * power(last_delta, MOD - 2) % MOD;
        vector<ll> fix(i - last_fail - 1, 0);
        fix.push_back(coef);
        for (ll x : last) fix.push_back((MOD - x) * coef % MOD);
        if (fix.size() < cur.size()) fix.resize(cur.size(), 0);
        for (size_t j = 0; j < cur.size(); j++) fix[j] = (fix[j] + cur[j]) % MOD;
        if ((ll)i - last_fail + (ll)last.size() >= (ll)cur.size()) last = cur, last_fail = i, last_delta = delta;
        cur = fix;
    }
    return cur;
}
// @end
