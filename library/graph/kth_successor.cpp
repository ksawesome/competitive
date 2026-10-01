// @name   K-th successor (binary lifting / jumping)
// @prefix cpjump
// @key    t .
// @desc   [global] Jump J(f, max_k); J.go(v, k) = f applied k times to v, for k <= max_k. Works for any functional graph.
// @usage  Jump J(f, 1e9);
// @usage  int where = J.go(v, k);
// @complexity O(n log K) build, O(log K) per query
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Jump {
    int LOG = 1;
    vector<vector<int>> up;  // up[j][v] = 2^j steps from v
    Jump(const vector<int>& f, ll max_k) {
        while ((1LL << LOG) <= max_k) LOG++;
        up.assign(LOG, f);
        for (int j = 1; j < LOG; j++)
            for (int v = 0; v < (int)f.size(); v++) up[j][v] = up[j - 1][up[j - 1][v]];
    }
    int go(int v, ll k) const {
        for (int j = 0; k; j++, k >>= 1)
            if (k & 1) v = up[j][v];
        return v;
    }
};
// @end
