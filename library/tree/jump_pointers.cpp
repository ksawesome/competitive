// @name   Jump pointers (level ancestors with one extra pointer per vertex)
// @prefix cpjumpptr
// @key    u j
// @desc   [global] JumpPointers jp(n); jp.add_root(r); jp.add_leaf(v, p) as vertices arrive (parent first); jp.kth(v, k), jp.ancestor_at_depth(v, d), jp.lca(a, b). Works online while the tree grows.
// @usage  JumpPointers jp(n); jp.add_root(0);
// @usage  for (int v : order) if (v != 0) jp.add_leaf(v, par[v]);  // any order with parents before children
// @usage  int a = jp.kth(v, k);  // -1 if v has fewer than k ancestors
// @complexity O(1) per added leaf, O(log n) per query
#include <bits/stdc++.h>
using namespace std;

// @begin
struct JumpPointers {
    vector<int> par, jump, depth;
    JumpPointers(int n) : par(n, -1), jump(n, -1), depth(n, 0) {}
    void add_root(int r) { par[r] = jump[r] = r, depth[r] = 0; }
    void add_leaf(int v, int p) {  // p must already be in the tree
        par[v] = p, depth[v] = depth[p] + 1;
        int j = jump[p];  // jump lengths follow the skew-binary number system
        jump[v] = depth[p] - depth[j] == depth[j] - depth[jump[j]] ? jump[j] : p;
    }
    int ancestor_at_depth(int v, int d) const {  // requires 0 <= d <= depth[v]
        while (depth[v] > d) v = depth[jump[v]] >= d ? jump[v] : par[v];
        return v;
    }
    int kth(int v, int k) const { return k > depth[v] ? -1 : ancestor_at_depth(v, depth[v] - k); }
    int lca(int a, int b) const {
        if (depth[a] < depth[b]) swap(a, b);
        a = ancestor_at_depth(a, depth[b]);
        while (a != b) {  // equal depths have equal jump lengths
            if (jump[a] != jump[b]) a = jump[a], b = jump[b];
            else a = par[a], b = par[b];
        }
        return a;
    }
};
// @end
