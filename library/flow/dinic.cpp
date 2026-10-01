// @name   Max flow (Dinic) + min cut
// @prefix cpdinic
// @key    y y
// @desc   [global] Dinic g(n); g.add_edge(u, v, cap); ll f = g.max_flow(s, t); after it g.min_cut_side(v) = v on the source side of a min cut.
// @usage  Dinic g(n); int e = g.add_edge(u, v, cap);
// @usage  ll f = g.max_flow(s, t); ll flow_on_e = g.flow_on(e);
// @complexity O(V^2 E) worst case, O(E sqrt V) on unit-capacity graphs
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Dinic {
    struct E {
        int to;
        ll cap, orig;
    };
    vector<E> e;
    vector<vector<int>> g;
    vector<int> level, it;
    Dinic(int n) : g(n), level(n), it(n) {}
    int add_edge(int u, int v, ll cap, ll rev_cap = 0) {  // returns the edge id
        g[u].push_back(e.size()), e.push_back({v, cap, cap});
        g[v].push_back(e.size()), e.push_back({u, rev_cap, rev_cap});
        return e.size() - 2;
    }
    ll max_flow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (ll f = dfs(s, t, LLONG_MAX)) flow += f;
        }
        return flow;
    }
    bool min_cut_side(int v) const { return level[v] >= 0; }  // valid right after max_flow
    ll flow_on(int id) const { return e[id].orig - e[id].cap; }

   private:
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int id : g[u])
                if (e[id].cap > 0 && level[e[id].to] < 0) level[e[id].to] = level[u] + 1, q.push(e[id].to);
        }
        return level[t] >= 0;
    }
    ll dfs(int u, int t, ll f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); i++) {
            int id = g[u][i], v = e[id].to;
            if (e[id].cap <= 0 || level[v] != level[u] + 1) continue;
            if (ll d = dfs(v, t, min(f, e[id].cap))) {
                e[id].cap -= d, e[id ^ 1].cap += d;
                return d;
            }
        }
        return 0;
    }
};
// @end
