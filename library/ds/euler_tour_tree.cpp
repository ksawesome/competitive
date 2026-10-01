// @name   Euler tour tree (dynamic forest: link, cut, connectivity, component sums)
// @prefix cpett
// @key    u e
// @desc   [global] ETT f(n); f.link(u, v), f.cut(u, v) (the edge must exist), f.connected(u, v), f.set(v, x), f.comp_sum(v), f.comp_size(v). Each tree is stored as its Euler tour in a treap.
// @usage  ETT f(n); f.link(0, 1); f.set(1, 5);
// @usage  bool c = f.connected(0, 1); ll s = f.comp_sum(0); f.cut(0, 1);
// @complexity O(log n) expected per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct ETT {
    // node v + 1 stands for vertex v; other nodes are directed edges (u, v). A tree's tour visits every vertex once
    // and every edge in both directions, so cutting an edge cuts the tour into three pieces.
    struct Nd { int l = 0, r = 0, p = 0, sz = 0, vc = 0; bool vert = false; unsigned pri = 0; ll val = 0, sum = 0; };
    vector<Nd> t;
    map<pair<int, int>, int> edge_node;
    mt19937 rng{912367};
    ETT(int n) : t(n + 1) {
        for (int v = 1; v <= n; v++) t[v].sz = t[v].vc = 1, t[v].vert = true, t[v].pri = rng();
    }
    void pull(int x) {
        const Nd &a = t[t[x].l], &b = t[t[x].r];
        t[x].sz = 1 + a.sz + b.sz;
        t[x].vc = t[x].vert + a.vc + b.vc;  // number of vertices (edges do not count)
        t[x].sum = t[x].val + a.sum + b.sum;
        if (t[x].l) t[t[x].l].p = x;
        if (t[x].r) t[t[x].r].p = x;
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (t[a].pri > t[b].pri) {
            int r = merge(t[a].r, b);
            t[a].r = r, pull(a);
            return a;
        }
        int l = merge(a, t[b].l);
        t[b].l = l, pull(b);
        return b;
    }
    pair<int, int> split(int x, int k) {  // first k nodes of the tour, the rest; both returned roots have p = 0
        if (!x) return {0, 0};
        if (t[t[x].l].sz >= k) {
            auto [a, b] = split(t[x].l, k);
            t[x].l = b, pull(x), t[x].p = 0;
            if (a) t[a].p = 0;
            return {a, x};
        }
        auto [a, b] = split(t[x].r, k - t[t[x].l].sz - 1);
        t[x].r = a, pull(x), t[x].p = 0;
        if (b) t[b].p = 0;
        return {x, b};
    }
    int root(int x) const {
        while (t[x].p) x = t[x].p;
        return x;
    }
    int index(int x) const {  // position of node x in its tour
        int i = t[t[x].l].sz;
        for (; t[x].p; x = t[x].p)
            if (t[t[x].p].r == x) i += t[t[t[x].p].l].sz + 1;
        return i;
    }
    int reroot(int v) {  // rotate v's tour so that it starts at v; returns the new root
        int x = v + 1, r = root(x);
        auto [a, b] = split(r, index(x));
        int nr = merge(b, a);
        t[nr].p = 0;
        return nr;
    }
    bool connected(int u, int v) const { return root(u + 1) == root(v + 1); }
    int new_edge(int u, int v) {
        Nd nd;
        nd.sz = 1, nd.pri = rng();
        t.push_back(nd);
        return edge_node[{u, v}] = t.size() - 1;
    }
    void link(int u, int v) {  // u and v must be in different trees
        int ru = reroot(u), rv = reroot(v);
        int e1 = new_edge(u, v), e2 = new_edge(v, u);
        int r = merge(merge(merge(ru, e1), rv), e2);
        t[r].p = 0;
    }
    void cut(int u, int v) {  // the edge u-v must exist
        reroot(u);
        int e1 = edge_node[{u, v}], e2 = edge_node[{v, u}];
        int i1 = index(e1), i2 = index(e2);  // the tour is  ... (u,v) [v's side] (v,u) ...
        auto [a, bc] = split(root(e1), i1);
        auto [b, c] = split(bc, i2 - i1 + 1);
        auto [e1n, rest] = split(b, 1);
        auto [vside, e2n] = split(rest, t[rest].sz - 1);
        (void)e1n, (void)e2n, (void)vside;
        int r = merge(a, c);
        if (r) t[r].p = 0;
        edge_node.erase({u, v}), edge_node.erase({v, u});
    }
    void set(int v, ll x) {
        int n = v + 1;
        t[n].val = x;
        for (; n; n = t[n].p) t[n].sum = t[n].val + t[t[n].l].sum + t[t[n].r].sum;
    }
    ll comp_sum(int v) const { return t[root(v + 1)].sum; }
    int comp_size(int v) const { return t[root(v + 1)].vc; }
};
// @end
