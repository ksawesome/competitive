// @name   Closest pair of points
// @prefix cpclosest
// @key    y 9
// @desc   [global] closest_pair(pts) = smallest squared distance between two of the points (n >= 2), sweep by x with a set by y. Needs cppoint.
// @usage  ll d2 = closest_pair(pts);
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "point.cpp"

// @begin
ll closest_pair(vector<Point> pts) {
    sort(pts.begin(), pts.end());
    set<pair<ll, ll>> active;  // {y, x} of points within the current x window
    ll best = LLONG_MAX;
    for (int i = 0, j = 0; i < (int)pts.size(); i++) {
        ll d = (ll)ceil(sqrt((long double)best));
        for (; j < i && pts[i].x - pts[j].x > d; j++) active.erase({pts[j].y, pts[j].x});
        auto lo = active.lower_bound({pts[i].y - d, LLONG_MIN}), hi = active.upper_bound({pts[i].y + d, LLONG_MAX});
        for (auto it = lo; it != hi; it++) {
            ll dx = pts[i].x - it->second, dy = pts[i].y - it->first;
            best = min(best, dx * dx + dy * dy);
        }
        active.insert({pts[i].y, pts[i].x});
    }
    return best;
}
// @end
