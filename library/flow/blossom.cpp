// @name   Maximum matching in a general graph (Edmonds' blossom)
// @prefix cpblossom
// @key    u shift+g
// @desc   [global] blossom_matching(adj): match[v] = partner or -1, for any undirected graph (odd cycles allowed). Matching size = number of v with match[v] != -1, divided by 2.
// @usage  vector<int> mate = blossom_matching(adj);
// @complexity O(n^3)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<int> blossom_matching(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> match(n, -1), p(n), base(n), q;
    vector<char> used(n), blossom(n);
    auto lca = [&](int a, int b) {  // first common base on the two alternating paths back to the root
        vector<char> seen(n, 0);
        while (true) {
            a = base[a], seen[a] = 1;
            if (match[a] == -1) break;
            a = p[match[a]];
        }
        while (true) {
            b = base[b];
            if (seen[b]) return b;
            b = p[match[b]];
        }
    };
    auto mark_path = [&](int v, int b, int child) {
        while (base[v] != b) {
            blossom[base[v]] = blossom[base[match[v]]] = 1;
            p[v] = child, child = match[v], v = p[match[v]];
        }
    };
    auto find_path = [&](int root) -> int {  // BFS for an augmenting path, shrinking odd cycles (blossoms) on the way
        fill(used.begin(), used.end(), 0), fill(p.begin(), p.end(), -1);
        iota(base.begin(), base.end(), 0);
        used[root] = 1, q.assign(1, root);
        for (size_t h = 0; h < q.size(); h++) {
            int v = q[h];
            for (int to : adj[v]) {
                if (base[v] == base[to] || match[v] == to) continue;
                if (to == root || (match[to] != -1 && p[match[to]] != -1)) {  // an odd cycle: contract it
                    int b = lca(v, to);
                    fill(blossom.begin(), blossom.end(), 0);
                    mark_path(v, b, to), mark_path(to, b, v);
                    for (int i = 0; i < n; i++)
                        if (blossom[base[i]]) {
                            base[i] = b;
                            if (!used[i]) used[i] = 1, q.push_back(i);
                        }
                } else if (p[to] == -1) {
                    p[to] = v;
                    if (match[to] == -1) return to;  // augmenting path found
                    used[match[to]] = 1, q.push_back(match[to]);
                }
            }
        }
        return -1;
    };
    for (int i = 0; i < n; i++)
        if (match[i] == -1)
            for (int v = find_path(i); v != -1;) {  // flip the path
                int pv = p[v], ppv = match[pv];
                match[v] = pv, match[pv] = v, v = ppv;
            }
    return match;
}
// @end
