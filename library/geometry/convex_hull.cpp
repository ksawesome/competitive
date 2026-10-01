// @name   Convex hull (monotone chain)
// @prefix cphull
// @key    t shift+v
// @desc   [global] convex_hull(pts) -> counter-clockwise hull without collinear points. Pt = pair<ll, ll>.
// @usage  vector<Pt> hull = convex_hull(pts);
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
using Pt = pair<ll, ll>;
ll cross(Pt o, Pt a, Pt b) {  // > 0 if o->a->b turns left
    return (a.first - o.first) * (b.second - o.second) - (a.second - o.second) * (b.first - o.first);
}
vector<Pt> convex_hull(vector<Pt> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = pts.size(), k = 0;
    if (n < 3) return pts;
    vector<Pt> h(2 * n);
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(h[k - 2], h[k - 1], pts[i]) <= 0) k--;
        h[k++] = pts[i];
    }
    for (int i = n - 2, lower = k + 1; i >= 0; i--) {
        while (k >= lower && cross(h[k - 2], h[k - 1], pts[i]) <= 0) k--;
        h[k++] = pts[i];
    }
    h.resize(k - 1);
    return h;
}
// @end
