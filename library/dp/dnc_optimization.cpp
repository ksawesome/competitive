// @name   Divide & conquer DP optimization
// @prefix cpdnc
// @key    y q
// @desc   [global] dnc_opt(K, n, cost): min total cost splitting a[0, n) into K groups; cost(j, i) = cost of group a[j, i). Needs monotone optimal split points.
// @usage  ll best = dnc_opt(K, n, [&](int j, int i) { ll s = pre[i] - pre[j]; return s * s; });
// @complexity O(K n log n) cost calls
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class C>
ll dnc_opt(int K, int n, C cost) {
    const ll inf = LLONG_MAX / 4;
    vector<ll> prev(n + 1, inf), cur(n + 1, inf);
    prev[0] = 0;
    for (int k = 1; k <= K; k++) {
        fill(cur.begin(), cur.end(), inf);
        auto rec = [&](auto&& self, int l, int r, int optl, int optr) -> void {  // fill cur[l..r], split in [optl, optr]
            if (l > r) return;
            int mid = (l + r) / 2, best = optl;
            for (int j = optl; j <= min(mid - 1, optr); j++) {
                if (prev[j] >= inf) continue;
                ll v = prev[j] + cost(j, mid);
                if (v < cur[mid]) cur[mid] = v, best = j;
            }
            self(self, l, mid - 1, optl, best);
            self(self, mid + 1, r, best, optr);
        };
        rec(rec, 1, n, 0, n - 1);
        swap(prev, cur);
    }
    return prev[n];
}
// @end
