// @name   BFS (shortest path, unweighted)
// @prefix cpbfs
// @key    t b
// @desc   [inside solve] needs n, adj, src. dist[v] = -1 if unreachable; get_path(t) rebuilds the route.
// @usage  // after it: dist[v] (-1 = unreachable), par[v]
// @usage  vector<int> path = get_path(t);  // src ... t
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

void context(int n, vector<vector<int>>& adj, int src) {
    // @begin
    vector<int> dist(n, -1), par(n, -1);
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                par[v] = u;
                q.push(v);
            }
        }
    }
    auto get_path = [&](int t) {  // src -> t, empty if unreachable
        vector<int> path;
        if (dist[t] == -1) return path;
        for (int v = t; v != -1; v = par[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    };
    // @end
    (void)get_path;
}
