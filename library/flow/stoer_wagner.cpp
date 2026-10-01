// @name   Global minimum cut (Stoer-Wagner)
// @prefix cpstoerwagner
// @key    u shift+a
// @desc   [global] stoer_wagner(w): the minimum total weight of edges whose removal disconnects the graph, w = symmetric n x n weight matrix (n >= 2). No source or sink needed.
// @usage  ll cut = stoer_wagner(w);
// @complexity O(n^3)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll stoer_wagner(vector<vector<ll>> w) {
    int n = w.size();
    ll best = LLONG_MAX;
    vector<char> merged(n, 0);
    for (int phase = 0; phase < n - 1; phase++) {
        // maximum adjacency order: repeatedly add the vertex most tightly connected to the added set;
        // the cut separating the last vertex from the rest is a minimum cut between the last two added
        vector<ll> conn(n, 0);
        vector<char> added(n, 0);
        int prev = -1, last = -1;
        for (int step = 0; step < n - phase; step++) {
            int sel = -1;
            for (int v = 0; v < n; v++)
                if (!merged[v] && !added[v] && (sel < 0 || conn[v] > conn[sel])) sel = v;
            added[sel] = 1, prev = last, last = sel;
            for (int v = 0; v < n; v++) conn[v] += w[sel][v];
        }
        best = min(best, conn[last] - w[last][last]);  // weight of the cut ({last}, rest)
        for (int v = 0; v < n; v++) w[prev][v] += w[last][v], w[v][prev] = w[prev][v];  // contract last into prev
        merged[last] = 1;
    }
    return best;
}
// @end
