// @name   Rotating calipers (diameter and width of a convex polygon)
// @prefix cpcalipers
// @key    u 5
// @desc   [global] For a convex polygon h (CCW, no three collinear, e.g. from the convex hull template): hull_diameter2(h) = max squared distance between two vertices (exact); hull_width(h) = min distance between two parallel lines enclosing it.
// @usage  ll d2 = hull_diameter2(hull);   // hull: vector<pair<ll, ll>>
// @usage  long double w = hull_width(hull);
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll hull_diameter2(const vector<pair<ll, ll>>& h) {
    int n = h.size();
    auto d2 = [&](int a, int b) { ll dx = h[a].first - h[b].first, dy = h[a].second - h[b].second; return dx * dx + dy * dy; };
    auto area2 = [&](int a, int b, int c) {
        return (h[b].first - h[a].first) * (h[c].second - h[a].second) - (h[b].second - h[a].second) * (h[c].first - h[a].first);
    };
    if (n <= 1) return 0;
    if (n == 2) return d2(0, 1);
    ll best = 0;
    for (int i = 0, j = 1; i < n; i++) {  // for each edge (i, i+1), move j to the vertex farthest from it
        int ni = (i + 1) % n;
        while (area2(i, ni, (j + 1) % n) > area2(i, ni, j)) j = (j + 1) % n;
        best = max({best, d2(i, j), d2(ni, j)});
    }
    return best;
}
long double hull_width(const vector<pair<ll, ll>>& h) {
    int n = h.size();
    if (n <= 2) return 0;
    auto area2 = [&](int a, int b, int c) {
        return (h[b].first - h[a].first) * (h[c].second - h[a].second) - (h[b].second - h[a].second) * (h[c].first - h[a].first);
    };
    long double best = 1e300L;
    for (int i = 0, j = 1; i < n; i++) {  // the narrowest strip has one side flush with a polygon edge
        int ni = (i + 1) % n;
        while (area2(i, ni, (j + 1) % n) > area2(i, ni, j)) j = (j + 1) % n;
        long double len = hypotl((long double)(h[ni].first - h[i].first), (long double)(h[ni].second - h[i].second));
        best = min(best, area2(i, ni, j) / len);
    }
    return best;
}
// @end
