// @name   Kruskal reconstruction tree
// @prefix cpkrt
// @key    y k
// @desc   [global] KRT t(n, edges {u, v, w}): leaves 0..n-1, each merge makes node x with val[x] = w. Max edge on the MST path a-b = val[lca(a, b)].
// @usage  KRT t(n, edges); auto g = t.tree();  // then LCA l(g, g.size() - 1) if the graph is connected
// @usage  // vertices reachable from v using edges <= x = leaves under v's highest ancestor with val <= x
// @complexity O(m log m)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct KRT {
    vector<int> par;  // parent in the reconstruction tree, -1 for roots
    vector<ll> val;   // merge weight of internal nodes, 0 for leaves
    template <class E>
    KRT(int n, vector<E> edges) : par(n, -1), val(n) {
        sort(edges.begin(), edges.end(), [](const E& a, const E& b) { return get<2>(a) < get<2>(b); });
        vector<int> top(n);  // union-find over tree nodes; the root of a set is its current top node
        iota(top.begin(), top.end(), 0);
        auto root_of = [&](int x) {
            while (top[x] != x) x = top[x] = top[top[x]];
            return x;
        };
        for (auto& e : edges) {
            int a = root_of(get<0>(e)), b = root_of(get<1>(e));
            if (a == b) continue;
            int x = par.size();
            par.push_back(-1), val.push_back(get<2>(e)), top.push_back(x);
            par[a] = par[b] = x, top[a] = top[b] = x;
        }
    }
    vector<vector<int>> tree() const {  // undirected adjacency, e.g. for the LCA template
        vector<vector<int>> g(par.size());
        for (int v = 0; v < (int)par.size(); v++)
            if (par[v] != -1) g[v].push_back(par[v]), g[par[v]].push_back(v);
        return g;
    }
};
// @end
