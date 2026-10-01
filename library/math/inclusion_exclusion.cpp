// @name   Inclusion-exclusion (divisible by any)
// @prefix cpincexc
// @key    y 3
// @desc   [global] count_divisible_by_any(N, ds): how many x in [1, N] are divisible by at least one of ds (k <= ~20). Odd subsets add, even subtract.
// @usage  ll c = count_divisible_by_any(N, {2, 3, 5});
// @usage  // coprime to m: N - count_divisible_by_any(N, distinct primes of m)
// @complexity O(2^k k)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class T>
ll count_divisible_by_any(ll N, const vector<T>& ds) {
    int k = ds.size();
    ll total = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        ll l = 1;
        bool over = false;
        for (int i = 0; i < k && !over; i++) {
            if (!(mask >> i & 1)) continue;
            ll g = gcd(l, (ll)ds[i]);
            if (l / g > N / ds[i]) over = true;  // lcm already exceeds N
            else l = l / g * ds[i];
        }
        if (!over) total += (__builtin_popcountll(mask) & 1 ? 1 : -1) * (N / l);
    }
    return total;
}
// @end
