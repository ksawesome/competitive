// @name   Kruskal MST
// @prefix cpkruskal
// @key    t k
// @desc   [global] auto [cost, used] = kruskal(n, edges); edges = {u, v, w}. Spanning forest if disconnected.
// @usage  vector<array<ll, 3>> edges;  // {u, v, w}
// @usage  auto [cost, used] = kruskal(n, edges);
// @complexity O(m log m)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class E>
pair<ll, vector<E>> kruskal(int n, vector<E> edges) {
    sort(edges.begin(), edges.end(), [](const E& a, const E& b) { return get<2>(a) < get<2>(b); });
    vector<int> par(n);
    iota(par.begin(), par.end(), 0);
    auto root = [&](int x) {
        while (x != par[x]) x = par[x] = par[par[x]];
        return x;
    };
    ll cost = 0;
    vector<E> used;
    for (auto& e : edges) {
        int a = root(get<0>(e)), b = root(get<1>(e));
        if (a == b) continue;
        par[a] = b;
        cost += get<2>(e);
        used.push_back(e);
    }
    return {cost, used};  // connected <=> used.size() == n - 1
}
// @end
