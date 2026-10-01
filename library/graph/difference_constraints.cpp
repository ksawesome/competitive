// @name   System of difference constraints
// @prefix cpdiffcons
// @key    u shift+i
// @desc   [global] DiffConstraints dc(n); dc.add(u, v, w) means x_v - x_u <= w; dc.solve() -> a solution vector (all x <= 0, maximal), or empty if infeasible (a negative cycle).
// @usage  DiffConstraints dc(n); dc.add(a, b, 3);   // x_b <= x_a + 3
// @usage  vector<ll> x = dc.solve(); if (x.empty()) { /* impossible */ }
// @complexity O(n m) (Bellman-Ford from a virtual source)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct DiffConstraints {
    int n;
    vector<array<ll, 3>> e;  // edge u -> v with weight w encodes x_v <= x_u + w
    DiffConstraints(int n_) : n(n_) {}
    void add(int u, int v, ll w) { e.push_back({u, v, w}); }  // x_v - x_u <= w
    void add_eq(int u, int v, ll w) { add(u, v, w), add(v, u, -w); }  // x_v - x_u = w
    vector<ll> solve() const {
        vector<ll> d(n, 0);  // a virtual source with 0-weight edges to all: start every distance at 0
        for (int it = 0; it < n; it++) {
            bool changed = false;
            for (auto& [u, v, w] : e)
                if (d[u] + w < d[v]) d[v] = d[u] + w, changed = true;
            if (!changed) return d;  // shortest distances satisfy every constraint d_v <= d_u + w
        }
        return {};  // still relaxing after n rounds: a negative cycle, the constraints contradict each other
    }
};
// @end
