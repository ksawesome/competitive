// @name   Min-cost max-flow
// @prefix cpmcmf
// @key    y shift+y
// @desc   [global] MCMF g(n); g.add_edge(u, v, cap, cost); auto [flow, cost] = g.run(s, t, limit). Shortest paths by SPFA, so negative costs are fine.
// @usage  MCMF g(n); g.add_edge(u, v, 1, w);
// @usage  auto [flow, cost] = g.run(s, t);  // cheapest among maximum flows (or among flows of size limit)
// @complexity O(flow * V * E) worst case, far less in practice
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct MCMF {
    struct E {
        int to;
        ll cap, cost;
    };
    vector<E> e;
    vector<vector<int>> g;
    MCMF(int n) : g(n) {}
    void add_edge(int u, int v, ll cap, ll cost) {
        g[u].push_back(e.size()), e.push_back({v, cap, cost});
        g[v].push_back(e.size()), e.push_back({u, 0, -cost});
    }
    pair<ll, ll> run(int s, int t, ll limit = LLONG_MAX) {
        int n = g.size();
        ll flow = 0, cost = 0;
        while (flow < limit) {
            vector<ll> dist(n, LLONG_MAX);
            vector<int> pe(n, -1);
            vector<bool> inq(n);
            deque<int> q{s};
            dist[s] = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop_front(), inq[u] = false;
                for (int id : g[u]) {
                    const E& ed = e[id];
                    if (ed.cap > 0 && dist[u] + ed.cost < dist[ed.to]) {
                        dist[ed.to] = dist[u] + ed.cost, pe[ed.to] = id;
                        if (!inq[ed.to]) inq[ed.to] = true, q.push_back(ed.to);
                    }
                }
            }
            if (dist[t] == LLONG_MAX) break;
            ll f = limit - flow;
            for (int v = t; v != s; v = e[pe[v] ^ 1].to) f = min(f, e[pe[v]].cap);
            for (int v = t; v != s; v = e[pe[v] ^ 1].to) e[pe[v]].cap -= f, e[pe[v] ^ 1].cap += f;
            flow += f, cost += f * dist[t];
        }
        return {flow, cost};
    }
};
// @end
