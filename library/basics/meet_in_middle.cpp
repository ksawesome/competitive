// @name   Meet in the middle (subset sums)
// @prefix cpmitm
// @key    t shift+u
// @desc   [global] count_subsets_leq(a, X): number of subsets with sum <= X for n <= ~40, splitting the items in two halves.
// @usage  ll ways = count_subsets_leq(a, X);
// @usage  // exact sum X: count upper_bound - lower_bound of X - x in the sorted right half
// @complexity O(2^(n/2) n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class T>
ll count_subsets_leq(const vector<T>& a, ll X) {
    int n = a.size(), h = n / 2;
    auto sums = [&](int from, int to) {
        vector<ll> s{0};
        for (int i = from; i < to; i++) {
            int k = s.size();
            for (int j = 0; j < k; j++) s.push_back(s[j] + a[i]);
        }
        return s;
    };
    vector<ll> L = sums(0, h), R = sums(h, n);
    sort(R.begin(), R.end());
    ll cnt = 0;
    for (ll x : L) cnt += upper_bound(R.begin(), R.end(), X - x) - R.begin();
    return cnt;
}
// @end
