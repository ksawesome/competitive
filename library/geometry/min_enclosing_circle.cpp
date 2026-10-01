// @name   Minimum enclosing circle (Welzl, randomized)
// @prefix cpmincircle
// @key    u shift+m
// @desc   [global] min_enclosing_circle(pts): the smallest circle {cx, cy, r} containing all points (long double). Random shuffle + incremental construction.
// @usage  auto [cx, cy, r] = min_enclosing_circle(pts);   // pts: vector<pair<long double, long double>>
// @complexity O(n) expected
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
array<long double, 3> min_enclosing_circle(vector<pair<long double, long double>> p) {
    using LD = long double;
    const LD EPS = 1e-12;
    shuffle(p.begin(), p.end(), mt19937(chrono::steady_clock::now().time_since_epoch().count()));
    auto dist = [](LD ax, LD ay, LD bx, LD by) { return hypotl(ax - bx, ay - by); };
    auto inside = [&](const array<LD, 3>& c, const pair<LD, LD>& q) { return dist(c[0], c[1], q.first, q.second) <= c[2] + EPS * max<LD>(1, c[2]); };
    auto two = [&](const pair<LD, LD>& a, const pair<LD, LD>& b) {
        return array<LD, 3>{(a.first + b.first) / 2, (a.second + b.second) / 2, dist(a.first, a.second, b.first, b.second) / 2};
    };
    auto three = [&](const pair<LD, LD>& a, const pair<LD, LD>& b, const pair<LD, LD>& c) {  // circumcircle
        LD bx = b.first - a.first, by = b.second - a.second, cx = c.first - a.first, cy = c.second - a.second;
        LD d = 2 * (bx * cy - by * cx);
        if (fabsl(d) < EPS) {  // collinear: the circle on the farthest pair
            array<LD, 3> r = two(a, b);
            for (auto cand : {two(a, c), two(b, c)}) if (cand[2] > r[2]) r = cand;
            return r;
        }
        LD ux = (cy * (bx * bx + by * by) - by * (cx * cx + cy * cy)) / d, uy = (bx * (cx * cx + cy * cy) - cx * (bx * bx + by * by)) / d;
        return array<LD, 3>{ux + a.first, uy + a.second, hypotl(ux, uy)};
    };
    int n = p.size();
    if (n == 0) return {0, 0, 0};
    array<LD, 3> c{p[0].first, p[0].second, 0};
    for (int i = 1; i < n; i++)
        if (!inside(c, p[i])) {  // p[i] must be on the boundary of the circle of p[0..i]
            c = {p[i].first, p[i].second, 0};
            for (int j = 0; j < i; j++)
                if (!inside(c, p[j])) {  // p[i] and p[j] on the boundary
                    c = two(p[i], p[j]);
                    for (int k = 0; k < j; k++)
                        if (!inside(c, p[k])) c = three(p[i], p[j], p[k]);
                }
        }
    return c;
}
// @end
