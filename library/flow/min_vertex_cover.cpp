// @name   Bipartite minimum vertex cover / maximum independent set (Konig)
// @prefix cpkonig
// @key    u v
// @desc   [global] konig(nl, nr, adj): adj[left] = right neighbours. Returns {cover_left, cover_right}: a minimum vertex cover, whose size equals the maximum matching; the complement is a maximum independent set.
// @usage  auto [cl, cr] = konig(nl, nr, adj);   // vertices in the cover (bool per side)
// @complexity O(V E) (augmenting paths)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
pair<vector<bool>, vector<bool>> konig(int nl, int nr, const vector<vector<int>>& adj) {
    vector<int> ml(nl, -1), mr(nr, -1);
    vector<int> seen(nr, -1);
    auto augment = [&](auto&& self, int u, int stamp) -> bool {  // Kuhn's augmenting path from left vertex u
        for (int v : adj[u]) {
            if (seen[v] == stamp) continue;
            seen[v] = stamp;
            if (mr[v] == -1 || self(self, mr[v], stamp)) { ml[u] = v, mr[v] = u; return true; }
        }
        return false;
    };
    for (int u = 0; u < nl; u++) augment(augment, u, u);
    // Konig: walk alternating paths from the unmatched left vertices (non-matching edges left->right,
    // matching edges right->left). Cover = unvisited left + visited right.
    vector<bool> vl(nl, false), vr(nr, false);
    vector<int> st;
    for (int u = 0; u < nl; u++) if (ml[u] == -1) vl[u] = true, st.push_back(u);
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        for (int v : adj[u])
            if (!vr[v] && ml[u] != v) {
                vr[v] = true;
                if (mr[v] != -1 && !vl[mr[v]]) vl[mr[v]] = true, st.push_back(mr[v]);
            }
    }
    vector<bool> cl(nl), cr(nr);
    for (int u = 0; u < nl; u++) cl[u] = !vl[u];
    for (int v = 0; v < nr; v++) cr[v] = vr[v];
    return {cl, cr};
}
// @end
