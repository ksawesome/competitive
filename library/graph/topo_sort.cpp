// @name   Topological sort (Kahn)
// @prefix cptopo
// @key    t o
// @desc   [global] vector<int> order = topo_sort(adj); empty if the graph has a cycle.
// @usage  vector<int> order = topo_sort(adj);
// @usage  if (order.empty()) { /* the graph has a cycle */ }
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
vector<int> topo_sort(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> indeg(n), order;
    for (auto& nb : adj)
        for (int v : nb) indeg[v]++;
    queue<int> q;  // priority_queue<int, vector<int>, greater<>> -> lexicographically smallest order
    for (int v = 0; v < n; v++)
        if (indeg[v] == 0) q.push(v);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : adj[u])
            if (--indeg[v] == 0) q.push(v);
    }
    if ((int)order.size() < n) order.clear();  // cycle
    return order;
}
// @end
