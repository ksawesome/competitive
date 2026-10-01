// @name   Monotone minima (row minima when the argmin moves right)
// @prefix cpmonominima
// @key    u m
// @desc   [global] monotone_minima(n, m, f): for each row i, the leftmost column j minimizing f(i, j), given that this argmin is non-decreasing in i (e.g. a Monge matrix).
// @usage  vector<int> opt = monotone_minima(n, m, [&](int i, int j) { return cost(i, j); });
// @complexity O((n + m) log n) evaluations of f
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class F>
vector<int> monotone_minima(int n, int m, F f) {
    vector<int> opt(n, 0);
    // rows [r1, r2) may only use columns [c1, c2]; solve the middle row, split around its argmin
    auto solve = [&](auto&& self, int r1, int r2, int c1, int c2) -> void {
        if (r1 >= r2) return;
        int mid = (r1 + r2) / 2, best = c1;
        auto bv = f(mid, c1);
        for (int j = c1 + 1; j <= c2; j++) {
            auto v = f(mid, j);
            if (v < bv) bv = v, best = j;
        }
        opt[mid] = best;
        self(self, r1, mid, c1, best);
        self(self, mid + 1, r2, best, c2);
    };
    if (m > 0) solve(solve, 0, n, 0, m - 1);
    return opt;
}
// @end
