// @name   Li Chao tree (min of lines)
// @prefix cplichao
// @key    y shift+l
// @desc   [global] LiChao lc(lo, hi); lc.add(k, m); lc.query(x) = min of k*x + m for integer x in [lo, hi). Nodes created on demand.
// @usage  LiChao lc(-1e9, 1e9 + 1);
// @usage  lc.add(k, m); ll best = lc.query(x);
// @complexity O(log range) per add / query
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct LiChao {
    struct Ln {
        ll k, m;
        ll at(ll x) const { return k * x + m; }
    };
    struct Nd {
        Ln ln;
        int l, r;
    };
    vector<Nd> t;
    ll lo, hi;
    LiChao(ll lo_, ll hi_) : t{{{0, LLONG_MAX}, 0, 0}, {{0, LLONG_MAX}, 0, 0}}, lo(lo_), hi(hi_) {}  // t[1] = root
    void add(ll k, ll m) {
        Ln nw{k, m};
        int x = 1;
        for (ll a = lo, b = hi;;) {  // [a, b)
            ll mid = a + (b - a) / 2;
            bool left_better = nw.at(a) < t[x].ln.at(a), mid_better = nw.at(mid) < t[x].ln.at(mid);
            if (mid_better) swap(t[x].ln, nw);
            if (b - a == 1) return;
            bool go_left = left_better != mid_better;
            int child = go_left ? t[x].l : t[x].r;
            if (!child) {
                child = t.size();
                t.push_back({{0, LLONG_MAX}, 0, 0});
                (go_left ? t[x].l : t[x].r) = child;
            }
            x = child;
            if (go_left) b = mid;
            else a = mid;
        }
    }
    ll query(ll x) const {
        ll res = LLONG_MAX;
        int v = 1;
        for (ll a = lo, b = hi; v;) {
            res = min(res, t[v].ln.at(x));
            ll mid = a + (b - a) / 2;
            if (x < mid) v = t[v].l, b = mid;
            else v = t[v].r, a = mid;
        }
        return res;
    }
};
// @end
