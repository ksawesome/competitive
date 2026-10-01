// @name   Offline LCA (Tarjan)
// @prefix cpofflinelca
// @key    u shift+l
// @desc   [global] tarjan_lca(adj, root, qs): the LCA of every query pair, answered in one DFS with a DSU. Faster than binary lifting when all queries are known in advance and memory is tight.
// @usage  vector<int> ans = tarjan_lca(adj, 0, qs);   // qs: vector<pair<int, int>>
// @complexity O((n + q) alpha(n))
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<int> tarjan_lca(const vector<vector<int>>& adj, int root, const vector<pair<int, int>>& qs) {
    int n = adj.size(), q = qs.size();
    vector<vector<pair<int, int>>> at(n);  // queries touching each vertex: (other end, index)
    for (int i = 0; i < q; i++) at[qs[i].first].push_back({qs[i].second, i}), at[qs[i].second].push_back({qs[i].first, i});
    vector<int> dsu(n), anc(n), ans(q, -1);
    iota(dsu.begin(), dsu.end(), 0);
    auto find = [&](int x) {
        while (dsu[x] != x) x = dsu[x] = dsu[dsu[x]];
        return x;
    };
    vector<char> done(n, 0);
    vector<int> par(n, -1);
    vector<size_t> it(n, 0);
    vector<int> st{root};
    par[root] = root, anc[root] = root;
    while (!st.empty()) {
        int v = st.back();
        if (it[v] < adj[v].size()) {
            int w = adj[v][it[v]++];
            if (w != par[v] && par[w] < 0) par[w] = v, anc[w] = w, st.push_back(w);
            continue;
        }
        st.pop_back();
        done[v] = 1;  // every finished vertex u has anc[find(u)] = the deepest ancestor of u still on the stack
        for (auto [u, i] : at[v])
            if (done[u] && ans[i] < 0) ans[i] = anc[find(u)];
        if (v != root) {
            int p = par[v];
            dsu[find(v)] = find(p), anc[find(p)] = p;
        }
    }
    return ans;
}
// @end
