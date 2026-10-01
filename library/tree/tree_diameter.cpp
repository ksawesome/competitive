// @name   Tree diameter
// @prefix cpdiameter
// @key    t shift+d
// @desc   [global] auto [len, path] = tree_diameter(adj); two BFS passes, path lists the vertices end to end.
// @usage  auto [len, path] = tree_diameter(adj);
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

// @begin
pair<int, vector<int>> tree_diameter(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> dist(n), par(n);
    auto farthest = [&](int s) {
        fill(dist.begin(), dist.end(), -1);
        dist[s] = 0;
        par[s] = -1;
        queue<int> q;
        q.push(s);
        int last = s;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            last = u;  // BFS pops in distance order, so the last one is farthest
            for (int v : adj[u])
                if (dist[v] == -1) dist[v] = dist[u] + 1, par[v] = u, q.push(v);
        }
        return last;
    };
    int a = farthest(0), b = farthest(a);
    vector<int> path;
    for (int v = b; v != -1; v = par[v]) path.push_back(v);
    return {dist[b], path};
}
// @end
