// @name   Long-path decomposition: depth-indexed tree DP in O(n)
// @prefix cplongpath
// @key    u shift+n
// @desc   [inside solve] needs n, adj, root. f_v[d] = # vertices at distance d below v lives at buf[pos[v] + d], shared with the long child (shifted by one); dom[v] = the smallest d maximising f_v[d]. Edit the merge for other depth DPs.
// @usage  // the array of v has h[v] cells: buf[pos[v]] .. buf[pos[v] + h[v] - 1]
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

void context(int n, vector<vector<int>>& adj, int root) {
    // @begin
    vector<int> par(n, -1), order{root}, h(n, 1), lc(n, -1);
    order.reserve(n);
    for (int i = 0; i < (int)order.size(); i++)
        for (int v : adj[order[i]])
            if (v != par[order[i]]) par[v] = order[i], order.push_back(v);
    for (int i = n - 1; i > 0; i--) {  // heights and long children
        int v = order[i], p = par[v];
        if (h[v] + 1 > h[p]) h[p] = h[v] + 1, lc[p] = v;
    }
    vector<int> pos(n), buf(n, 0), dom(n);
    int next = 0;
    for (int v : order)  // one block per long path, each vertex one cell after its parent
        if (v == root || lc[par[v]] != v) pos[v] = next, next += h[v];
        else pos[v] = pos[par[v]] + 1;
    for (int i = n - 1; i >= 0; i--) {  // children first
        int v = order[i];
        auto f = buf.begin() + pos[v];
        f[0] = 1;  // the long child's counts are already in place, shifted by one
        dom[v] = lc[v] != -1 ? dom[lc[v]] + 1 : 0;
        if (f[dom[v]] <= 1) dom[v] = 0;
        for (int c : adj[v]) {  // short children are merged by hand: h[c] cells each
            if (c == par[v] || c == lc[v]) continue;
            for (int d = 0; d < h[c]; d++) {
                f[d + 1] += buf[pos[c] + d];
                if (f[d + 1] > f[dom[v]] || (f[d + 1] == f[dom[v]] && d + 1 < dom[v])) dom[v] = d + 1;
            }
        }
    }
    // @end
}
