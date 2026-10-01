// @name   Mo's algorithm on tree paths
// @prefix cpmotree
// @key    u o
// @desc   [global] mo_on_tree(adj, root, qs, add, remove, answer): offline queries on the vertex set of the path u-v. add(v)/remove(v) change the current set, answer(i) records query i. Uses the Euler tour trick (a vertex seen twice cancels).
// @usage  mo_on_tree(adj, 0, qs, [&](int v) { if (cnt[c[v]]++ == 0) distinct++; }, [&](int v) { if (--cnt[c[v]] == 0) distinct--; }, [&](int i) { ans[i] = distinct; });
// @complexity O((n + q) sqrt(n) * cost of add / remove + q log n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class Add, class Remove, class Answer>
void mo_on_tree(const vector<vector<int>>& adj, int root, const vector<pair<int, int>>& qs, Add add, Remove remove,
                Answer answer) {
    int n = adj.size(), LOG = 1, q = qs.size();
    while ((1 << LOG) < n) LOG++;
    vector<int> tin(n), tout(n), euler(2 * n), dep(n, 0);
    vector<vector<int>> up(LOG, vector<int>(n, root));
    // iterative DFS: each vertex enters and leaves the Euler sequence once
    vector<pair<int, int>> st{{root, -1}};
    vector<size_t> it(n, 0);
    int timer = 0;
    tin[root] = timer, euler[timer++] = root;
    while (!st.empty()) {
        auto [v, p] = st.back();
        if (it[v] < adj[v].size()) {
            int w = adj[v][it[v]++];
            if (w == p) continue;
            dep[w] = dep[v] + 1, up[0][w] = v;
            tin[w] = timer, euler[timer++] = w;
            st.push_back({w, v});
        } else {
            tout[v] = timer, euler[timer++] = v;
            st.pop_back();
        }
    }
    for (int j = 1; j < LOG; j++)
        for (int v = 0; v < n; v++) up[j][v] = up[j - 1][up[j - 1][v]];
    auto lca = [&](int a, int b) {
        if (dep[a] < dep[b]) swap(a, b);
        for (int j = LOG - 1; j >= 0; j--)
            if (dep[a] - (1 << j) >= dep[b]) a = up[j][a];
        if (a == b) return a;
        for (int j = LOG - 1; j >= 0; j--)
            if (up[j][a] != up[j][b]) a = up[j][a], b = up[j][b];
        return up[0][a];
    };
    // path u-v as a range of the Euler sequence: [tin u, tin v] if u is an ancestor, else [tout u, tin v] + the lca
    vector<array<int, 4>> ranges(q);  // l, r (inclusive), extra vertex or -1, query index
    for (int i = 0; i < q; i++) {
        auto [u, v] = qs[i];
        if (tin[u] > tin[v]) swap(u, v);
        int l = lca(u, v);
        ranges[i] = l == u ? array<int, 4>{tin[u], tin[v], -1, i} : array<int, 4>{tout[u], tin[v], l, i};
    }
    int block = max<int>(1, 2 * n / max(1.0, sqrt((double)q)));
    sort(ranges.begin(), ranges.end(), [&](const array<int, 4>& x, const array<int, 4>& y) {
        if (x[0] / block != y[0] / block) return x[0] < y[0];
        return (x[0] / block) % 2 ? x[1] > y[1] : x[1] < y[1];  // snake order halves the pointer movement
    });
    vector<char> in(n, 0);
    auto toggle = [&](int pos) {
        int v = euler[pos];
        if (in[v]) remove(v);
        else add(v);
        in[v] ^= 1;
    };
    int L = 0, R = -1;
    for (auto& [l, r, extra, idx] : ranges) {
        while (R < r) toggle(++R);
        while (L > l) toggle(--L);
        while (R > r) toggle(R--);
        while (L < l) toggle(L++);
        if (extra >= 0) add(extra);
        answer(idx);
        if (extra >= 0) remove(extra);
    }
}
// @end
