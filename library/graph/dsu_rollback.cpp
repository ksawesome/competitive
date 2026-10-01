// @name   DSU with rollback
// @prefix cpdsurb
// @key    y r
// @desc   [global] DSURollback d(n); d.unite(a, b); int t = d.snapshot(); ... d.rollback(t) undoes every unite after t. No path compression.
// @usage  DSURollback d(n);
// @usage  int t = d.snapshot(); d.unite(a, b); /* ... */ d.rollback(t);
// @complexity O(log n) per find / unite, O(1) per undone unite
#include <bits/stdc++.h>
using namespace std;

// @begin
struct DSURollback {
    vector<int> par, siz;
    vector<pair<int, int>> hist;  // {attached root, root it was attached to}
    int comps;
    DSURollback(int n) : par(n), siz(n, 1), comps(n) { iota(par.begin(), par.end(), 0); }
    int find(int x) const {
        while (x != par[x]) x = par[x];
        return x;
    }
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (siz[a] < siz[b]) swap(a, b);
        hist.push_back({b, a});
        par[b] = a, siz[a] += siz[b], comps--;
        return true;
    }
    bool same(int a, int b) const { return find(a) == find(b); }
    int snapshot() const { return hist.size(); }
    void rollback(int snap) {
        while ((int)hist.size() > snap) {
            auto [b, a] = hist.back();
            hist.pop_back();
            par[b] = b, siz[a] -= siz[b], comps++;
        }
    }
};
// @end
