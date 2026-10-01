// @name   Bipartite check (2-coloring)
// @prefix cpbipartite
// @key    t 2
// @desc   [global] vector<int> color = bipartite(adj); colors are 0/1, empty if an odd cycle exists.
// @usage  vector<int> color = bipartite(adj);
// @usage  if (color.empty()) { /* odd cycle: not bipartite */ }
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
vector<int> bipartite(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> color(n, -1);
    for (int s = 0; s < n; s++) {
        if (color[s] != -1) continue;
        color[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = color[u] ^ 1;
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return {};
                }
            }
        }
    }
    return color;
}
// @end
