// @name   Bellman-Ford (negative edges)
// @prefix cpbellman
// @key    t shift+w
// @desc   [global] bellman_ford(n, edges, src), edges = {u, v, w}. BF_INF = unreachable, -BF_INF = hit by a negative cycle.
// @usage  vector<array<ll, 3>> edges;  // {u, v, w}
// @usage  vector<ll> dist = bellman_ford(n, edges, src);  // -BF_INF: negative cycle reaches it
// @complexity O(nm)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
const ll BF_INF = LLONG_MAX / 4;
template <class E>
vector<ll> bellman_ford(int n, const vector<E>& edges, int src) {
    vector<ll> dist(n, BF_INF);
    dist[src] = 0;
    for (int round = 0; round < n; round++) {  // an improvement in round n means a negative cycle
        for (auto& [u, v, w] : edges) {
            if (dist[u] == BF_INF || dist[u] == -BF_INF) continue;
            if (dist[u] + w < dist[v]) dist[v] = (round == n - 1) ? -BF_INF : dist[u] + w;
        }
    }
    for (int round = 0; round < n; round++)  // spread -inf to everything reachable from a negative cycle
        for (auto& [u, v, w] : edges)
            if (dist[u] == -BF_INF) dist[v] = -BF_INF;
    return dist;
}
// @end
