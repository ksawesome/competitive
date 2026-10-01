// @name   Heavy-light decomposition
// @prefix cphld
// @key    y h
// @desc   [global] HLD h(adj, root); h.for_path(a, b, f) calls f(l, r) on position ranges [l, r) covering the path; pair with any [l, r) segment tree.
// @usage  HLD h(adj); SegTree st(n);                      // st.set(h.pos[v], value of v)
// @usage  ll s = 0; h.for_path(a, b, [&](int l, int r) { s += st.query(l, r); });
// @usage  // subtree of v = [h.pos[v], h.pos[v] + h.sub[v]); h.lca(a, b)
// @complexity O(n) build, O(log n) ranges per path
#include <bits/stdc++.h>
using namespace std;

// @begin
struct HLD {
    vector<int> par, depth, heavy, head, pos, sub;
    HLD(const vector<vector<int>>& adj, int root = 0) {
        int n = adj.size();
        par.assign(n, -1), depth.assign(n, 0), heavy.assign(n, -1), head.assign(n, 0), pos.assign(n, 0), sub.assign(n, 1);
        vector<int> order{root};
        order.reserve(n);
        for (int i = 0; i < (int)order.size(); i++)
            for (int v : adj[order[i]])
                if (v != par[order[i]]) par[v] = order[i], depth[v] = depth[order[i]] + 1, order.push_back(v);
        for (int i = n - 1; i > 0; i--) sub[par[order[i]]] += sub[order[i]];
        for (int u : order)
            for (int v : adj[u])
                if (v != par[u] && (heavy[u] == -1 || sub[v] > sub[heavy[u]])) heavy[u] = v;
        int timer = 0;
        vector<int> st{root};
        while (!st.empty()) {  // walk each heavy chain from its head; light children start new chains
            int h = st.back();
            st.pop_back();
            for (int v = h; v != -1; v = heavy[v]) {
                head[v] = h, pos[v] = timer++;
                for (int c : adj[v])
                    if (c != par[v] && c != heavy[v]) st.push_back(c);
            }
        }
    }
    template <class F>
    void for_path(int a, int b, F f) const {  // vertex values; for edge values skip pos[lca] (see the last line)
        for (; head[a] != head[b]; b = par[head[b]]) {
            if (depth[head[a]] > depth[head[b]]) swap(a, b);
            f(pos[head[b]], pos[b] + 1);
        }
        if (depth[a] > depth[b]) swap(a, b);
        f(pos[a], pos[b] + 1);  // edge values: f(pos[a] + 1, pos[b] + 1)
    }
    int lca(int a, int b) const {
        for (; head[a] != head[b]; b = par[head[b]])
            if (depth[head[a]] > depth[head[b]]) swap(a, b);
        return depth[a] < depth[b] ? a : b;
    }
};
// @end
