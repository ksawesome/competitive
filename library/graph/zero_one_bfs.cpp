// @name   0-1 BFS
// @prefix cp01bfs
// @key    t 0
// @desc   [global] vector<ll> dist = zero_one_bfs(adj, src); adj[u] = {v, w} with w in {0, 1}. Unreachable = LLONG_MAX.
// @usage  vector<ll> dist = zero_one_bfs(adj, src);  // adj[u] = {v, w}, w in {0, 1}
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class G>
vector<ll> zero_one_bfs(const G& adj, int src) {
    int n = adj.size();
    vector<ll> dist(n, LLONG_MAX);
    deque<int> dq;
    dist[src] = 0;
    dq.push_back(src);
    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist;
}
// @end
