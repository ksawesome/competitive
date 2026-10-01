// @name   Eulerian path / circuit (Hierholzer)
// @prefix cpeuler
// @key    y e
// @desc   [global] euler_path(n, edges, directed, src = -1): vertex sequence using every edge exactly once, or empty if impossible.
// @usage  vector<int> path = euler_path(n, edges, false);       // path.size() == m + 1
// @usage  vector<int> tour = euler_path(n, edges, false, 0);    // circuit from 0 (only when every degree is even)
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
vector<int> euler_path(int n, const vector<pair<int, int>>& edges, bool directed, int src = -1) {
    int m = edges.size();
    vector<vector<pair<int, int>>> g(n);  // {to, edge id}
    vector<int> deg(n);                   // directed: out - in; undirected: degree
    for (int i = 0; i < m; i++) {
        auto [u, v] = edges[i];
        g[u].push_back({v, i});
        if (directed) deg[u]++, deg[v]--;
        else g[v].push_back({u, i}), deg[u]++, deg[v]++;
    }
    int start = -1;
    if (directed) {
        int plus = 0, minus = 0;
        for (int v = 0; v < n; v++) {
            if (deg[v] == 1) plus++, start = v;
            else if (deg[v] == -1) minus++;
            else if (deg[v] != 0) return {};
        }
        if (plus != minus || plus > 1) return {};
    } else {
        int odd = 0;
        for (int v = 0; v < n; v++)
            if (deg[v] % 2) odd++, start = v;
        if (odd != 0 && odd != 2) return {};
    }
    if (start == -1) start = src;  // a circuit can start anywhere that has an edge
    if (start == -1)
        for (int v = 0; v < n && start == -1; v++)
            if (!g[v].empty()) start = v;
    if (start == -1) return {};
    vector<int> path, st{start};
    vector<size_t> ptr(n);
    vector<bool> used(m);
    while (!st.empty()) {
        int u = st.back();
        while (ptr[u] < g[u].size() && used[g[u][ptr[u]].second]) ptr[u]++;
        if (ptr[u] == g[u].size()) {
            path.push_back(u);
            st.pop_back();
        } else {
            auto [v, id] = g[u][ptr[u]];
            used[id] = true;
            st.push_back(v);
        }
    }
    if ((int)path.size() != m + 1) return {};  // edges not all connected
    reverse(path.begin(), path.end());
    return path;
}
// @end
