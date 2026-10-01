// @name   Bipartite matching (Hopcroft-Karp)
// @prefix cphopcroft
// @key    y shift+h
// @desc   [global] HopcroftKarp hk(nl, nr); hk.add_edge(u, v) with u on the left, v on the right; hk.max_matching(); hk.match_l[u] = partner or -1.
// @usage  HopcroftKarp hk(nl, nr); hk.add_edge(u, v);
// @usage  int m = hk.max_matching();  // Konig: min vertex cover = m; max independent set = nl + nr - m
// @complexity O(E sqrt V)
#include <bits/stdc++.h>
using namespace std;

// @begin
struct HopcroftKarp {
    int nl, nr;
    vector<vector<int>> g;
    vector<int> match_l, match_r, dist;
    HopcroftKarp(int nl_, int nr_) : nl(nl_), nr(nr_), g(nl_), match_l(nl_, -1), match_r(nr_, -1), dist(nl_) {}
    void add_edge(int u, int v) { g[u].push_back(v); }
    int max_matching() {
        int res = 0;
        while (bfs())
            for (int u = 0; u < nl; u++)
                if (match_l[u] == -1 && dfs(u)) res++;
        return res;
    }

   private:
    bool bfs() {  // layer the free left vertices; true if some augmenting path exists
        queue<int> q;
        for (int u = 0; u < nl; u++) {
            dist[u] = match_l[u] == -1 ? 0 : -1;
            if (!dist[u]) q.push(u);
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                int w = match_r[v];
                if (w == -1) found = true;
                else if (dist[w] == -1) dist[w] = dist[u] + 1, q.push(w);
            }
        }
        return found;
    }
    bool dfs(int u) {
        for (int v : g[u]) {
            int w = match_r[v];
            if (w == -1 || (dist[w] == dist[u] + 1 && dfs(w))) {
                match_l[u] = v, match_r[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }
};
// @end
