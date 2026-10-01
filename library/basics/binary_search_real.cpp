// @name   Binary search on a real answer
// @prefix cpbsreal
// @key    t shift+f
// @desc   [inside solve] smallest real x in [lo, hi] with ok(x); ok must be monotone. 100 halvings reach full double precision.
// @usage  // fill in ok(x); lo ends within 1e-15 of the boundary (relative to hi - lo)
// @complexity 100 calls to ok
#include <bits/stdc++.h>
using namespace std;
using ld = long double;

void context() {
    // @begin
    auto ok = [&](ld x) -> bool {
        return x * x >= 2;  // replace with the real check (this one finds sqrt(2))
    };
    ld lo = 0, hi = 1e9;
    for (int it = 0; it < 100; it++) {
        ld mid = (lo + hi) / 2;
        if (ok(mid)) hi = mid;
        else lo = mid;
    }
    // hi is the answer
    // @end
}
