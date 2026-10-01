// @name   Offline dynamic connectivity (edge deletions)
// @prefix cpofflinecc
// @key    y shift+r
// @desc   [global] offline_components(n, ops): ops are {0, u, v} add edge, {1, u, v} remove it, {2, 0, 0} query; returns #components per query. Needs cpdsurb.
// @usage  vector<array<int, 3>> ops;  // {type, u, v}
// @usage  vector<int> answers = offline_components(n, ops);
// @complexity O(q log q log n)
#include <bits/stdc++.h>
using namespace std;
#include "dsu_rollback.cpp"

// @begin
vector<int> offline_components(int n, const vector<array<int, 3>>& ops) {
    int q = ops.size(), size = 1;
    while (size < q) size <<= 1;
    vector<vector<pair<int, int>>> tree(2 * size);  // segment tree over time: node = edges alive on its whole range
    auto add = [&](auto&& self, int x, int lo, int hi, int l, int r, pair<int, int> e) -> void {
        if (r <= lo || hi <= l) return;
        if (l <= lo && hi <= r) return tree[x].push_back(e);
        int mid = (lo + hi) / 2;
        self(self, 2 * x, lo, mid, l, r, e);
        self(self, 2 * x + 1, mid, hi, l, r, e);
    };
    map<pair<int, int>, vector<int>> open;  // edge -> times it was added and not yet removed
    for (int i = 0; i < q; i++) {
        auto [t, u, v] = ops[i];
        pair<int, int> e = minmax(u, v);
        if (t == 0) open[e].push_back(i);
        else if (t == 1) add(add, 1, 0, size, open[e].back(), i, e), open[e].pop_back();
    }
    for (auto& [e, starts] : open)
        for (int s : starts) add(add, 1, 0, size, s, q, e);
    DSURollback d(n);
    vector<int> res;
    auto walk = [&](auto&& self, int x, int lo, int hi) -> void {  // apply node's edges, recurse, undo
        if (lo >= q) return;
        int snap = d.snapshot();
        for (auto [u, v] : tree[x]) d.unite(u, v);
        if (hi - lo == 1) {
            if (ops[lo][0] == 2) res.push_back(d.comps);
        } else {
            int mid = (lo + hi) / 2;
            self(self, 2 * x, lo, mid);
            self(self, 2 * x + 1, mid, hi);
        }
        d.rollback(snap);
    };
    if (q) walk(walk, 1, 0, size);
    return res;
}
// @end
