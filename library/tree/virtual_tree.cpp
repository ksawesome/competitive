// @name   Virtual (auxiliary) tree
// @prefix cpvirtual
// @key    y v
// @desc   [global] virtual_tree(nodes, tin, lca) -> {vertices, parent}: key nodes + pairwise LCAs, parent[i] = virtual parent of vertices[i] (-1 root).
// @usage  auto [vs, vpar] = virtual_tree(keys, tin, [&](int a, int b) { return t.lca(a, b); });  // tin from cptree, t from cplca
// @usage  // then DP over vs in reverse order: children appear after their parents
// @complexity O(k log k) LCA calls
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class F>
pair<vector<int>, vector<int>> virtual_tree(vector<int> nodes, const vector<int>& tin, F lca) {
    auto by_tin = [&](int a, int b) { return tin[a] < tin[b]; };
    sort(nodes.begin(), nodes.end(), by_tin);
    int k = nodes.size();
    for (int i = 0; i + 1 < k; i++) nodes.push_back(lca(nodes[i], nodes[i + 1]));
    sort(nodes.begin(), nodes.end(), by_tin);
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
    vector<int> parent(nodes.size(), -1);
    for (int i = 1; i < (int)nodes.size(); i++) parent[i] = lca(nodes[i - 1], nodes[i]);
    return {nodes, parent};
}
// @end
