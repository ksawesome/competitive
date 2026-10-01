// @name   Prufer code (tree <-> sequence)
// @prefix cpprufer
// @key    u u
// @desc   [global] prufer_encode(n, edges): the Prufer sequence (length n-2) of a labelled tree on 0..n-1; prufer_decode(seq): the tree back. Bijection behind Cayley's formula n^(n-2); vertex v appears deg(v)-1 times.
// @usage  vector<int> seq = prufer_encode(n, edges);
// @usage  vector<pair<int, int>> tree = prufer_decode(seq);
// @complexity O(n) (linear pointer technique)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<int> prufer_encode(int n, const vector<pair<int, int>>& edges) {
    vector<vector<int>> adj(n);
    for (auto [u, v] : edges) adj[u].push_back(v), adj[v].push_back(u);
    vector<int> par(n, -1), deg(n), order;  // root at n-1: the last vertex is never removed
    vector<int> st{n - 1};
    par[n - 1] = n - 1;
    while (!st.empty()) {
        int v = st.back();
        st.pop_back();
        for (int w : adj[v]) if (par[w] < 0) par[w] = v, st.push_back(w);
    }
    for (int v = 0; v < n; v++) deg[v] = adj[v].size();
    vector<int> seq;
    int ptr = 0;
    while (ptr < n && deg[ptr] != 1) ptr++;
    int leaf = ptr;
    for (int i = 0; i < n - 2; i++) {  // remove the smallest leaf each time
        int p = par[leaf];
        seq.push_back(p);
        if (--deg[p] == 1 && p < ptr) leaf = p;  // the parent became a leaf smaller than the pointer
        else {
            ptr++;
            while (ptr < n && deg[ptr] != 1) ptr++;
            leaf = ptr;
        }
    }
    return seq;
}
vector<pair<int, int>> prufer_decode(const vector<int>& seq) {
    int n = seq.size() + 2;
    vector<int> deg(n, 1);
    for (int v : seq) deg[v]++;
    int ptr = 0;
    while (deg[ptr] != 1) ptr++;
    int leaf = ptr;
    vector<pair<int, int>> edges;
    for (int v : seq) {
        edges.push_back({leaf, v});
        if (--deg[v] == 1 && v < ptr) leaf = v;
        else {
            ptr++;
            while (deg[ptr] != 1) ptr++;
            leaf = ptr;
        }
    }
    edges.push_back({leaf, n - 1});
    return edges;
}
// @end
