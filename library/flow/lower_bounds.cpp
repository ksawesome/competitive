// @name   Max flow with lower bounds
// @prefix cpflowlb
// @key    y shift+g
// @desc   [global] LowerBoundFlow lb(n); lb.add_edge(u, v, low, high); lb.max_flow(s, t) = max s-t flow meeting every bound, -1 if none. Needs cpdinic.
// @usage  LowerBoundFlow lb(n); lb.add_edge(u, v, 1, 3);
// @usage  ll f = lb.max_flow(s, t);  // -1: the bounds cannot be met
// @complexity one or two Dinic runs
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "dinic.cpp"

// @begin
struct LowerBoundFlow {
    int n;
    Dinic d;
    vector<ll> excess;  // forced inflow - forced outflow per vertex
    LowerBoundFlow(int n_) : n(n_), d(n_ + 2), excess(n_) {}
    void add_edge(int u, int v, ll low, ll high) {
        d.add_edge(u, v, high - low);
        excess[v] += low, excess[u] -= low;
    }
    ll max_flow(int s, int t) {  // call once
        int S = n, T = n + 1;
        ll need = 0;
        for (int v = 0; v < n; v++) {
            if (excess[v] > 0) d.add_edge(S, v, excess[v]), need += excess[v];
            else if (excess[v] < 0) d.add_edge(v, T, -excess[v]);
        }
        int back = d.add_edge(t, s, LLONG_MAX / 4);  // turns the s-t flow into a circulation
        if (d.max_flow(S, T) != need) return -1;
        ll base = d.flow_on(back);
        d.e[back].cap = d.e[back ^ 1].cap = 0;  // remove the helper edge, then push more flow s -> t
        return base + d.max_flow(s, t);
    }
};
// @end
