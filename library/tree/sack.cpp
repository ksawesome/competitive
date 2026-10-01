// @name   DSU on tree (Sack): subtree statistics in one shared array
// @prefix cpsack
// @key    u shift+t
// @desc   [inside solve] needs n, adj, root, color (values in [0, n)). Keeps the heavy child's counts and re-adds the light subtrees: best[v] = the largest count of one color in v's subtree. Edit add / clear / the answer line.
// @usage  // add(v) puts one vertex into cnt[] and the statistic; clear(v) empties the subtree of v again
// @complexity O(n log n) calls of add
#include <bits/stdc++.h>
using namespace std;

void context(int n, vector<vector<int>>& adj, int root, vector<int>& color) {
    // @begin
    vector<int> par(n, -1), order{root}, sub(n, 1), heavy(n, -1);
    order.reserve(n);
    for (int i = 0; i < (int)order.size(); i++)
        for (int v : adj[order[i]])
            if (v != par[order[i]]) par[v] = order[i], order.push_back(v);
    for (int i = n - 1; i > 0; i--) sub[par[order[i]]] += sub[order[i]];
    for (int v : order)
        for (int c : adj[v])
            if (c != par[v] && (heavy[v] == -1 || sub[c] > sub[heavy[v]])) heavy[v] = c;
    vector<int> tin(n), ver, st{root};  // preorder: the subtree of v is ver[tin[v] .. tin[v] + sub[v])
    while (!st.empty()) {
        int v = st.back();
        st.pop_back();
        tin[v] = ver.size(), ver.push_back(v);
        for (int c : adj[v])
            if (c != par[v]) st.push_back(c);
    }
    vector<int> cnt(n, 0), best(n);
    int top = 0;  // the statistic: the largest count of one color
    auto add = [&](int v) { top = max(top, ++cnt[color[v]]); };
    auto clear = [&](int v) {  // remove the whole subtree of v
        for (int t = tin[v]; t < tin[v] + sub[v]; t++) cnt[color[ver[t]]]--;
        top = 0;
    };
    vector<array<int, 3>> todo{{root, 1, 0}};  // (vertex, keep its counts, children done)
    while (!todo.empty()) {
        auto [v, keep, done] = todo.back();
        todo.pop_back();
        if (!done) {  // light children are popped first, the heavy child last
            todo.push_back({v, keep, 1});
            if (heavy[v] != -1) todo.push_back({heavy[v], 1, 0});
            for (int c : adj[v])
                if (c != par[v] && c != heavy[v]) todo.push_back({c, 0, 0});
            continue;
        }
        for (int c : adj[v])  // the heavy child's counts are still there
            if (c != par[v] && c != heavy[v])
                for (int t = tin[c]; t < tin[c] + sub[c]; t++) add(ver[t]);
        add(v);
        best[v] = top;  // cnt[] now describes exactly the subtree of v
        if (!keep) clear(v);
    }
    // @end
}
