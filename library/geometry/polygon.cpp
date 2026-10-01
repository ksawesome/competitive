// @name   Polygon: area, point inside, lattice points
// @prefix cppolygon
// @key    y 8
// @desc   [global] area2(poly) = 2 * signed area (> 0 counter-clockwise); in_polygon(poly, p): 1 inside, 0 boundary, -1 outside; boundary_points. Needs cppoint.
// @usage  ll twice = llabs(area2(poly)); int where = in_polygon(poly, p);
// @usage  ll interior = (twice - boundary_points(poly) + 2) / 2;  // Pick's theorem
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "point.cpp"

// @begin
ll area2(const vector<Point>& poly) {
    ll s = 0;
    for (int i = 0, n = poly.size(); i < n; i++) s += poly[i].cross(poly[(i + 1) % n]);
    return s;
}
int in_polygon(const vector<Point>& poly, Point p) {
    bool inside = false;
    for (int i = 0, n = poly.size(); i < n; i++) {
        Point a = poly[i], b = poly[(i + 1) % n];
        if (on_segment(p, a, b)) return 0;
        if (a.y > b.y) swap(a, b);
        if (a.y <= p.y && p.y < b.y && orient(a, b, p) > 0) inside = !inside;  // edge crosses the ray to the right
    }
    return inside ? 1 : -1;
}
ll boundary_points(const vector<Point>& poly) {
    ll s = 0;
    for (int i = 0, n = poly.size(); i < n; i++) {
        Point d = poly[(i + 1) % n] - poly[i];
        s += gcd(llabs(d.x), llabs(d.y));
    }
    return s;
}
// @end
