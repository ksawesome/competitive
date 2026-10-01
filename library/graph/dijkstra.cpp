// @name   Dijkstra
// @prefix cpdijkstra
// @key    t j
// @desc   [global] auto [dist, par] = dijkstra(adj, src); adj[u] = {v, w}, w >= 0. Unreachable = LLONG_MAX.
// @usage  vector<vector<pair<int, ll>>> adj(n);  // adj[u].push_back({v, w})
// @usage  auto [dist, par] = dijkstra(adj, src);
// @complexity O((n + m) log n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class G>
pair<vector<ll>, vector<int>> dijkstra(const G& adj, int src) {
    int n = adj.size();
    vector<ll> dist(n, LLONG_MAX);
    vector<int> par(n, -1);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;  // stale entry
        for (auto [v, w] : adj[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                par[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return {dist, par};
}
// @end
