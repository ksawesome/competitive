// @name   Rectangles (intersection, union of two)
// @prefix cprect
// @key    t shift+4
// @desc   [global] Rect {x1, y1, x2, y2} (x1 < x2, y1 < y2): area(), intersect_area(a, b), union_area(a, b). USACO bronze staples.
// @usage  Rect a{0, 0, 4, 3}, b{2, 1, 6, 5};
// @usage  ll both = intersect_area(a, b), either = union_area(a, b);
// @complexity O(1)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Rect {
    ll x1, y1, x2, y2;
    ll area() const { return (x2 - x1) * (y2 - y1); }
};
ll intersect_area(const Rect& a, const Rect& b) {
    ll w = max(0LL, min(a.x2, b.x2) - max(a.x1, b.x1));
    ll h = max(0LL, min(a.y2, b.y2) - max(a.y1, b.y1));
    return w * h;
}
ll union_area(const Rect& a, const Rect& b) { return a.area() + b.area() - intersect_area(a, b); }
// @end
