// @name   Binary search on the answer
// @prefix cpbsearch
// @key    t a
// @desc   [inside solve] smallest x in [lo, hi] with ok(x); ok must be monotone (false...false true...true).
// @usage  // fill in ok(x); lo ends as the smallest x with ok(x)
// @complexity O(log(hi - lo)) calls to ok
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context() {
    // @begin
    auto ok = [&](ll x) -> bool {
        return x >= 0;  // replace with the real check
    };
    ll lo = 0, hi = 2e18;  // answer lies in [lo, hi]; hi is returned if nothing smaller works
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        if (ok(mid)) hi = mid;
        else lo = mid + 1;
    }
    // lo is the answer
    // @end
}
