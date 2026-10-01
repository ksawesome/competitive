// @name   Floyd-Warshall (all pairs)
// @prefix cpfloyd
// @key    t w
// @desc   [global] floyd_warshall(d) in place, O(n^3). d[i][i] < 0 afterwards <=> i is on a negative cycle.
// @usage  vector<vector<ll>> d(n, vector<ll>(n, FW_INF));
// @usage  for (int i = 0; i < n; i++) d[i][i] = 0;  // then d[u][v] = min(d[u][v], w) per edge
// @usage  floyd_warshall(d);
// @complexity O(n^3)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
// Before: d[i][i] = 0, d[u][v] = edge weight (keep the min for multi-edges), FW_INF if no edge.
//   vector<vector<ll>> d(n, vector<ll>(n, FW_INF)); for (int i = 0; i < n; i++) d[i][i] = 0;
const ll FW_INF = LLONG_MAX / 4;
void floyd_warshall(vector<vector<ll>>& d) {
    int n = d.size();
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++) {
            if (d[i][k] >= FW_INF) continue;
            for (int j = 0; j < n; j++)
                if (d[k][j] < FW_INF) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        }
}
// @end
