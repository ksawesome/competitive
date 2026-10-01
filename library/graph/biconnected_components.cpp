// @name   Biconnected components (blocks)
// @prefix cpbcc
// @key    y shift+b
// @desc   [global] biconnected_components(n, edges): every block (maximal subgraph with no cut vertex) as a vertex list. Cut vertices appear in several.
// @usage  auto blocks = biconnected_components(n, edges);
// @usage  // block-cut tree: node per block + node per cut vertex, edge when the cut vertex is in the block
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
vector<vector<int>> biconnected_components(int n, const vector<pair<int, int>>& edges) {
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v] = edges[i];
        if (u == v) continue;  // self-loops never matter
        g[u].push_back({v, i});
        g[v].push_back({u, i});
    }
    vector<int> tin(n, -1), low(n), stk;
    vector<vector<int>> blocks;
    int timer = 0;
    auto dfs = [&](auto&& self, int u, int parent_edge) -> void {
        tin[u] = low[u] = timer++;
        stk.push_back(u);
        for (auto [v, id] : g[u]) {
            if (id == parent_edge) continue;
            if (tin[v] != -1) {
                low[u] = min(low[u], tin[v]);
                continue;
            }
            self(self, v, id);
            low[u] = min(low[u], low[v]);
            if (low[v] >= tin[u]) {  // u separates v's subtree: that subtree plus u is a block
                vector<int> block{u};
                for (int x = -1; x != v;) x = stk.back(), stk.pop_back(), block.push_back(x);
                blocks.push_back(block);
            }
        }
    };
    for (int v = 0; v < n; v++) {
        if (tin[v] != -1) continue;
        size_t before = blocks.size();
        dfs(dfs, v, -1);
        if (blocks.size() == before) blocks.push_back({v});  // isolated vertex
        stk.clear();
    }
    return blocks;
}
// @end
