// @name   Ternary search (unimodal max)
// @prefix cpternary
// @key    t q
// @desc   [global] argmax_int(lo, hi, f) / argmax_real(lo, hi, f) for f that rises then falls. For a minimum pass -f.
// @usage  ll x = argmax_int(0, 1e9, [&](ll x) { return profit(x); });
// @usage  ld y = argmax_real(0, 1e4, [&](ld x) { return -cost(x); });  // minimum of cost
// @complexity O(log range) calls to f
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;

// @begin
template <class F>
ll argmax_int(ll lo, ll hi, F f) {  // f strictly increases, then strictly decreases (flat only at the top)
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        if (f(mid) < f(mid + 1)) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}
template <class F>
ld argmax_real(ld lo, ld hi, F f) {
    for (int it = 0; it < 200; it++) {
        ld m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2)) lo = m1;
        else hi = m2;
    }
    return (lo + hi) / 2;
}
// @end
