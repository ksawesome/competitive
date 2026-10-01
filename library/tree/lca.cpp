// @name   LCA (binary lifting)
// @prefix cplca
// @key    t l
// @desc   [global] LCA t(adj, root); t.lca(a, b), t.dist(a, b), t.kth_ancestor(v, k), t.depth[v].
// @usage  LCA t(adj, root);
// @usage  t.lca(a, b); t.dist(a, b); t.kth_ancestor(v, k); t.depth[v];
// @complexity O(n log n) build, O(log n) per query
#include <bits/stdc++.h>
using namespace std;

// @begin
struct LCA {
    int n, LOG;
    vector<vector<int>> up;
    vector<int> depth;
    LCA(const vector<vector<int>>& adj, int root = 0) : n(adj.size()), LOG(1), depth(n) {
        while ((1 << LOG) < n) LOG++;
        up.assign(LOG, vector<int>(n, root));
        vector<int> order{root};  // BFS order: no recursion
        vector<bool> seen(n);
        seen[root] = true;
        for (int i = 0; i < (int)order.size(); i++) {
            int u = order[i];
            for (int v : adj[u]) {
                if (seen[v]) continue;
                seen[v] = true;
                up[0][v] = u;
                depth[v] = depth[u] + 1;
                order.push_back(v);
            }
        }
        for (int k = 1; k < LOG; k++)
            for (int v = 0; v < n; v++) up[k][v] = up[k - 1][up[k - 1][v]];
    }
    int kth_ancestor(int v, int k) {  // -1 if k > depth[v]
        if (k > depth[v]) return -1;
        for (int j = 0; j < LOG; j++)
            if (k >> j & 1) v = up[j][v];
        return v;
    }
    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        a = kth_ancestor(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; k--)
            if (up[k][a] != up[k][b]) a = up[k][a], b = up[k][b];
        return up[0][a];
    }
    int dist(int a, int b) { return depth[a] + depth[b] - 2 * depth[lca(a, b)]; }
};
// @end
