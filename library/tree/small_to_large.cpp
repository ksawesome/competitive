// @name   Small-to-large merging
// @prefix cpsmall2large
// @key    y m
// @desc   [inside solve] needs n, adj, root, color. distinct[v] = # distinct colors in v's subtree; always pour the smaller set into the larger.
// @usage  // swap set<int> for map<int, int> to keep counts (e.g. most frequent color per subtree)
// @complexity O(n log^2 n)
#include <bits/stdc++.h>
using namespace std;

void context(int n, vector<vector<int>>& adj, int root, vector<int>& color) {
    // @begin
    vector<int> par(n, -1), order{root};
    order.reserve(n);
    for (int i = 0; i < (int)order.size(); i++)
        for (int v : adj[order[i]])
            if (v != par[order[i]]) par[v] = order[i], order.push_back(v);
    vector<set<int>> bag(n);
    vector<int> distinct(n);
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        bag[u].insert(color[u]);
        for (int c : adj[u]) {
            if (c == par[u]) continue;
            if (bag[c].size() > bag[u].size()) swap(bag[c], bag[u]);  // O(1) swap keeps the big set in place
            for (int x : bag[c]) bag[u].insert(x);
            set<int>().swap(bag[c]);
        }
        distinct[u] = bag[u].size();
    }
    // @end
}
