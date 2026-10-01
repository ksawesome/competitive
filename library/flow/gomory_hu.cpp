// @name   Gomory-Hu tree (all-pairs minimum cut)
// @prefix cpgomoryhu
// @key    u a
// @desc   [global] gomory_hu(n, edges): a tree par[v], w[v] (v >= 1) on 0..n-1 for an undirected capacitated graph; the min cut between any u and v equals the smallest w on the tree path (Gusfield's algorithm, n - 1 max flows).
// @usage  auto [par, w] = gomory_hu(n, edges);   // edges: {u, v, capacity}
// @complexity n - 1 max flows (Dinic)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
pair<vector<int>, vector<ll>> gomory_hu(int n, const vector<array<ll, 3>>& edges) {
    struct E { int to; ll cap; };
    vector<E> es;
    vector<vector<int>> g(n);
    for (auto& [u, v, c] : edges) {  // undirected edge = two arcs, each with capacity c, paired as i and i ^ 1
        g[u].push_back(es.size()), es.push_back({(int)v, c});
        g[v].push_back(es.size()), es.push_back({(int)u, c});
    }
    vector<ll> orig(es.size());
    for (size_t i = 0; i < es.size(); i++) orig[i] = es[i].cap;
    vector<int> level(n), it(n);
    auto bfs = [&](int s) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> qu;
        qu.push(s);
        while (!qu.empty()) {
            int v = qu.front();
            qu.pop();
            for (int id : g[v])
                if (es[id].cap > 0 && level[es[id].to] < 0) level[es[id].to] = level[v] + 1, qu.push(es[id].to);
        }
    };
    auto dfs = [&](auto&& self, int v, int t, ll f) -> ll {
        if (v == t) return f;
        for (; it[v] < (int)g[v].size(); it[v]++) {
            int id = g[v][it[v]], to = es[id].to;
            if (es[id].cap > 0 && level[to] == level[v] + 1) {
                ll d = self(self, to, t, min(f, es[id].cap));
                if (d > 0) {
                    es[id].cap -= d, es[id ^ 1].cap += d;
                    return d;
                }
            }
        }
        return 0;
    };
    vector<int> par(n, 0);
    vector<ll> w(n, 0);
    for (int s = 1; s < n; s++) {
        int t = par[s];
        for (size_t i = 0; i < es.size(); i++) es[i].cap = orig[i];
        ll flow = 0;
        while (true) {
            bfs(s);
            if (level[t] < 0) break;
            fill(it.begin(), it.end(), 0);
            while (ll f = dfs(dfs, s, t, LLONG_MAX)) flow += f;
        }
        w[s] = flow;  // after the last BFS, level[v] >= 0 marks the s side of a minimum s-t cut
        for (int v = s + 1; v < n; v++)
            if (level[v] >= 0 && par[v] == t) par[v] = s;
    }
    return {par, w};
}
// @end
