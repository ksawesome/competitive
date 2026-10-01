// @name   Half-plane intersection
// @prefix cphalfplane
// @key    u 4
// @desc   [global] half_plane_intersection(hs, B): hs[i] = {ax, ay, bx, by} keeps the points LEFT of the directed line a->b. Returns the intersection polygon (CCW, long double) clipped to the box [-B, B]^2, or empty if it is empty.
// @usage  vector<array<long double, 4>> hs = {{0, 0, 1, 0}, {1, 0, 0, 1}, {0, 1, 0, 0}};   // a triangle
// @usage  auto poly = half_plane_intersection(hs, 1e9);
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<pair<long double, long double>> half_plane_intersection(vector<array<long double, 4>> hs, long double B = 1e9) {
    using LD = long double;
    using Pt2 = pair<LD, LD>;
    const LD EPS = 1e-12;
    struct HP { Pt2 p, d; LD ang; };  // point on the line, direction; keep the left side
    auto cross = [](Pt2 a, Pt2 b) { return a.first * b.second - a.second * b.first; };
    auto sub = [](Pt2 a, Pt2 b) { return Pt2{a.first - b.first, a.second - b.second}; };
    auto out = [&](const HP& h, Pt2 r) { return cross(h.d, sub(r, h.p)) < -EPS; };  // r strictly right of h
    auto inter = [&](const HP& s, const HP& t) {  // intersection point of the two boundary lines
        LD alpha = cross(sub(t.p, s.p), t.d) / cross(s.d, t.d);
        return Pt2{s.p.first + s.d.first * alpha, s.p.second + s.d.second * alpha};
    };
    Pt2 box[4] = {{B, B}, {-B, B}, {-B, -B}, {B, -B}};  // counter-clockwise: its inside is on the left of each edge
    for (int i = 0; i < 4; i++) hs.push_back({box[i].first, box[i].second, box[(i + 1) % 4].first, box[(i + 1) % 4].second});
    vector<HP> H;
    for (auto& [ax, ay, bx, by] : hs) H.push_back({{ax, ay}, {bx - ax, by - ay}, atan2l(by - ay, bx - ax)});
    sort(H.begin(), H.end(), [](const HP& a, const HP& b) { return a.ang < b.ang; });
    deque<HP> dq;
    for (auto& h : H) {
        while (dq.size() > 1 && out(h, inter(dq[dq.size() - 1], dq[dq.size() - 2]))) dq.pop_back();
        while (dq.size() > 1 && out(h, inter(dq[0], dq[1]))) dq.pop_front();
        if (!dq.empty() && fabsl(cross(h.d, dq.back().d)) < EPS) {  // parallel to the last one
            if (h.d.first * dq.back().d.first + h.d.second * dq.back().d.second < 0) return {};  // opposite: empty
            if (out(h, dq.back().p)) dq.pop_back();  // same direction: keep the stricter one
            else continue;
        }
        dq.push_back(h);
    }
    while (dq.size() > 2 && out(dq[0], inter(dq[dq.size() - 1], dq[dq.size() - 2]))) dq.pop_back();
    while (dq.size() > 2 && out(dq[dq.size() - 1], inter(dq[0], dq[1]))) dq.pop_front();
    if (dq.size() < 3) return {};
    vector<Pt2> poly;
    for (size_t i = 0; i < dq.size(); i++) poly.push_back(inter(dq[i], dq[(i + 1) % dq.size()]));
    return poly;
}
// @end
