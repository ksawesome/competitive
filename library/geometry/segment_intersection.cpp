// @name   Segment intersection
// @prefix cpseginter
// @key    y 7
// @desc   [global] segments_intersect(a, b, c, d): closed segments ab and cd share a point (touching and overlapping count). Needs cppoint.
// @usage  bool hit = segments_intersect(a, b, c, d);
// @complexity O(1)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "point.cpp"

// @begin
bool segments_intersect(Point a, Point b, Point c, Point d) {
    int o1 = orient(a, b, c), o2 = orient(a, b, d), o3 = orient(c, d, a), o4 = orient(c, d, b);
    if (o1 * o2 < 0 && o3 * o4 < 0) return true;  // proper crossing
    return on_segment(c, a, b) || on_segment(d, a, b) || on_segment(a, c, d) || on_segment(b, c, d);
}
// @end
