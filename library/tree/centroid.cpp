// @name   Centroid decomposition
// @prefix cpcentroid
// @key    y c
// @desc   [global] CentroidDecomp cd(adj): cd.cpar[v] = parent in the centroid tree (-1 at the top), cd.clevel[v] = its depth (<= log2 n).
// @usage  CentroidDecomp cd(adj);
// @usage  for (int c = v; c != -1; c = cd.cpar[c]) { /* every path through v passes a centroid ancestor c: use dist(v, c) */ }
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;

// @begin
struct CentroidDecomp {
    const vector<vector<int>>& adj;
    vector<int> cpar, clevel, sub, tpar;
    vector<bool> removed;
    CentroidDecomp(const vector<vector<int>>& g)
        : adj(g), cpar(g.size(), -1), clevel(g.size()), sub(g.size()), tpar(g.size()), removed(g.size()) {
        if (!g.empty()) build(0, -1, 0);
    }
    void build(int root, int parent, int level) {
        vector<int> order{root};  // this component, BFS order from root
        tpar[root] = -1;
        for (int i = 0; i < (int)order.size(); i++)
            for (int v : adj[order[i]])
                if (!removed[v] && v != tpar[order[i]]) tpar[v] = order[i], order.push_back(v);
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int u = order[i];
            sub[u] = 1;
            for (int v : adj[u])
                if (!removed[v] && v != tpar[u]) sub[u] += sub[v];
        }
        int total = order.size(), c = root;
        for (bool moved = true; moved;) {  // step into any child holding more than half
            moved = false;
            for (int v : adj[c])
                if (!removed[v] && v != tpar[c] && 2 * sub[v] > total) {
                    c = v, moved = true;
                    break;
                }
        }
        cpar[c] = parent, clevel[c] = level, removed[c] = true;
        for (int v : adj[c])
            if (!removed[v]) build(v, c, level + 1);
    }
};
// @end
