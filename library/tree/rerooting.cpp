// @name   Rerooting (DP for every root)
// @prefix cpreroot
// @key    t '
// @desc   [global] reroot<T>(adj, id, merge, lift): res[v] = merge over v's neighbours w of lift(value of w's side, w). merge associative, id its identity.
// @usage  // farthest vertex from every v:  auto far = reroot<ll>(adj, 0, [](ll a, ll b) { return max(a, b); }, [](ll x, int) { return x + 1; });
// @usage  // sum of distances: T = pair<ll, ll> {sum, count}, lift(p, w) = {p.first + p.second + 1, p.second + 1}, merge = add
// @complexity O(n) merges and lifts
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T, class Merge, class Lift>
vector<T> reroot(const vector<vector<int>>& adj, T id, Merge merge, Lift lift) {
    int n = adj.size();
    vector<int> par(n, -1), order{0};
    order.reserve(n);
    for (int i = 0; i < (int)order.size(); i++)
        for (int v : adj[order[i]])
            if (v != par[order[i]]) par[v] = order[i], order.push_back(v);
    vector<T> down(n, id), up(n, id), res(n, id);  // down: own subtree, up: everything above
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        for (int c : adj[u])
            if (c != par[u]) down[u] = merge(down[u], lift(down[c], c));
    }
    for (int u : order) {
        vector<int> ch;
        for (int c : adj[u])
            if (c != par[u]) ch.push_back(c);
        int k = ch.size();
        vector<T> pre(k + 1, id), suf(k + 1, id);
        for (int j = 0; j < k; j++) pre[j + 1] = merge(pre[j], lift(down[ch[j]], ch[j]));
        for (int j = k - 1; j >= 0; j--) suf[j] = merge(lift(down[ch[j]], ch[j]), suf[j + 1]);
        res[u] = merge(up[u], pre[k]);
        for (int j = 0; j < k; j++) up[ch[j]] = lift(merge(up[u], merge(pre[j], suf[j + 1])), u);
    }
    return res;
}
// @end
