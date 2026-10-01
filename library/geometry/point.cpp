// @name   Geometry primitives (integer points)
// @prefix cppoint
// @key    t shift+3
// @desc   [global] Point {x, y} with + - * dot cross norm2; orient(a, b, c) = +1 left turn, -1 right, 0 collinear; on_segment(p, a, b). Exact.
// @usage  Point a{1, 2}, b, c; cin >> b >> c;
// @usage  int o = orient(a, b, c); bool on = on_segment(p, a, b); ll d2 = (a - b).norm2();
// @complexity O(1)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Point {
    ll x = 0, y = 0;
    Point operator+(Point o) const { return {x + o.x, y + o.y}; }
    Point operator-(Point o) const { return {x - o.x, y - o.y}; }
    Point operator*(ll k) const { return {x * k, y * k}; }
    ll dot(Point o) const { return x * o.x + y * o.y; }
    ll cross(Point o) const { return x * o.y - y * o.x; }
    ll norm2() const { return x * x + y * y; }
    bool operator<(Point o) const { return make_pair(x, y) < make_pair(o.x, o.y); }
    bool operator==(Point o) const { return x == o.x && y == o.y; }
    friend istream& operator>>(istream& is, Point& p) { return is >> p.x >> p.y; }
};
int orient(Point a, Point b, Point c) {  // sign of cross(b - a, c - a)
    ll v = (b - a).cross(c - a);
    return (v > 0) - (v < 0);
}
bool on_segment(Point p, Point a, Point b) {
    return orient(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
// @end
