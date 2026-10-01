// @name   DSU (union-find)
// @prefix cpdsu
// @key    t u
// @desc   [global] DSU d(n); d.unite(a, b) -> false if already joined; d.same, d.size, d.comps.
// @usage  DSU d(n);
// @usage  if (d.unite(a, b)) { /* merged two components */ }
// @usage  d.same(a, b); d.size(a); d.comps;
// @complexity ~O(1) amortized per call
#include <bits/stdc++.h>
using namespace std;

// @begin
struct DSU {
    vector<int> par, siz;
    int comps;
    DSU(int n) : par(n), siz(n, 1), comps(n) { iota(par.begin(), par.end(), 0); }
    int find(int x) {
        while (x != par[x]) x = par[x] = par[par[x]];
        return x;
    }
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (siz[a] < siz[b]) swap(a, b);
        par[b] = a;
        siz[a] += siz[b];
        comps--;
        return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
    int size(int x) { return siz[find(x)]; }
};
// @end
