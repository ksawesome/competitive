// @name   Persistent implicit treap (rope with versions)
// @prefix cpptreap
// @key    u r
// @desc   [global] PTreap t; int v = t.build(a); every operation returns a NEW version and keeps the old: insert(v, pos, x), erase(v, pos), concat(v1, v2), slice(v, l, r). Queries: get(v, pos), sum(v, l, r), size(v).
// @usage  PTreap t; int v0 = t.build(a); int v1 = t.insert(v0, 2, 7);
// @usage  int copy_paste = t.concat(v1, t.slice(v1, l, r)); ll s = t.sum(v0, l, r);
// @usage  t.t.reserve(n + 50 * q);  // ~46 new 32-byte nodes per update; regrowing briefly holds two copies
// @complexity O(log n) expected per operation, O(log n) new nodes per update
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct PTreap {
    struct Nd { int l, r, sz; ll val, sum; };
    vector<Nd> t{{0, 0, 0, 0, 0}};  // node 0 = empty tree
    mt19937 rng{20260930};
    int size(int x) const { return t[x].sz; }
    int make(ll v) { t.push_back({0, 0, 1, v, v}); return t.size() - 1; }
    int copy(int x) { t.push_back(t[x]); return t.size() - 1; }
    int pull(int x) {
        t[x].sz = 1 + t[t[x].l].sz + t[t[x].r].sz;
        t[x].sum = t[x].val + t[t[x].l].sum + t[t[x].r].sum;
        return x;
    }
    // the root is chosen with probability proportional to size, so no priorities are stored (copies stay random)
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if ((int)(rng() % (unsigned)(t[a].sz + t[b].sz)) < t[a].sz) {
            int c = copy(a);
            int r = merge(t[c].r, b);
            t[c].r = r;
            return pull(c);
        }
        int c = copy(b);
        int l = merge(a, t[c].l);
        t[c].l = l;
        return pull(c);
    }
    pair<int, int> split(int x, int k) {  // first k elements, the rest
        if (!x) return {0, 0};
        int c = copy(x);
        if (t[t[c].l].sz >= k) {
            auto [a, b] = split(t[c].l, k);
            t[c].l = b;
            return {a, pull(c)};
        }
        auto [a, b] = split(t[c].r, k - t[t[c].l].sz - 1);
        t[c].r = a;
        return {pull(c), b};
    }
    int build(const vector<ll>& a, int l = 0, int r = -1) {
        if (r < 0) r = a.size();
        if (l >= r) return 0;
        int m = (l + r) / 2, x = make(a[m]);
        int left = build(a, l, m), right = build(a, m + 1, r);
        t[x].l = left, t[x].r = right;
        return pull(x);
    }
    int insert(int v, int pos, ll x) { auto [a, b] = split(v, pos); return merge(merge(a, make(x)), b); }
    int erase(int v, int pos) {
        auto [a, b] = split(v, pos);
        return merge(a, split(b, 1).second);
    }
    int concat(int v1, int v2) { return merge(v1, v2); }
    int slice(int v, int l, int r) { return split(split(v, r).first, l).second; }  // [l, r)
    ll get(int v, int pos) const {
        while (true) {
            int ls = t[t[v].l].sz;
            if (pos < ls) v = t[v].l;
            else if (pos == ls) return t[v].val;
            else pos -= ls + 1, v = t[v].r;
        }
    }
    ll prefix(int v, int k) const {  // sum of the first k elements, without copying
        ll s = 0;
        while (v && k > 0) {
            int ls = t[t[v].l].sz;
            if (k <= ls) v = t[v].l;
            else s += t[t[v].l].sum + t[v].val, k -= ls + 1, v = t[v].r;
        }
        return s;
    }
    ll sum(int v, int l, int r) const { return prefix(v, r) - prefix(v, l); }
};
// @end
