// @name   2-SAT
// @prefix cp2sat
// @key    y 2
// @desc   [global] TwoSat ts(n); ts.either(a, va, b, vb) adds the clause (x_a == va OR x_b == vb); ts.solve() -> values or empty if impossible.
// @usage  TwoSat ts(n);
// @usage  ts.either(a, true, b, false); ts.implies(a, true, c, true); ts.set(d, false);
// @usage  vector<int> x = ts.solve();  // empty: unsatisfiable
// @complexity O(n + clauses)
#include <bits/stdc++.h>
using namespace std;

// @begin
struct TwoSat {
    int n;
    vector<vector<int>> g;
    TwoSat(int n_) : n(n_), g(2 * n_) {}
    int node(int x, bool val) const { return 2 * x + (val ? 0 : 1); }  // literal "x == val"
    void either(int a, bool va, int b, bool vb) {
        g[node(a, !va)].push_back(node(b, vb));
        g[node(b, !vb)].push_back(node(a, va));
    }
    void implies(int a, bool va, int b, bool vb) { either(a, !va, b, vb); }
    void set(int a, bool va) { either(a, va, a, va); }
    vector<int> solve() {
        int N = 2 * n, timer = 0, ncomp = 0;
        vector<int> tin(N, -1), low(N), comp(N, -1), stk;
        auto dfs = [&](auto&& self, int u) -> void {  // Tarjan: ids come out in reverse topological order
            tin[u] = low[u] = timer++;
            stk.push_back(u);
            for (int v : g[u]) {
                if (tin[v] == -1) self(self, v), low[u] = min(low[u], low[v]);
                else if (comp[v] == -1) low[u] = min(low[u], tin[v]);
            }
            if (low[u] == tin[u]) {
                for (int v = -1; v != u;) v = stk.back(), stk.pop_back(), comp[v] = ncomp;
                ncomp++;
            }
        };
        for (int v = 0; v < N; v++)
            if (tin[v] == -1) dfs(dfs, v);
        vector<int> res(n);
        for (int x = 0; x < n; x++) {
            if (comp[2 * x] == comp[2 * x + 1]) return {};
            res[x] = comp[2 * x] < comp[2 * x + 1];
        }
        return res;
    }
};
// @end
