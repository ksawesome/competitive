// @name   Tree isomorphism (canonical IDs, AHU)
// @prefix cptreehash
// @key    u h
// @desc   [global] TreeCanon tc; tc.rooted(adj, root) and tc.unrooted(adj) give integer IDs: two trees get the same ID (from the same TreeCanon object) iff they are isomorphic. rooted_all(adj, root)[v] is the ID of v's subtree.
// @usage  TreeCanon tc;
// @usage  bool iso = tc.unrooted(adj1) == tc.unrooted(adj2);
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct TreeCanon {
    map<vector<int>, int> ids;  // sorted multiset of children IDs -> ID (shared across all trees)
    vector<int> rooted_all(const vector<vector<int>>& adj, int root) {
        int n = adj.size();
        vector<int> order{root}, par(n, -1), id(n);
        par[root] = root;
        for (size_t i = 0; i < order.size(); i++)
            for (int w : adj[order[i]]) if (par[w] < 0) par[w] = order[i], order.push_back(w);
        vector<vector<int>> ch(n);
        for (int i = n - 1; i >= 0; i--) {  // children before parents
            int v = order[i];
            sort(ch[v].begin(), ch[v].end());
            auto it = ids.find(ch[v]);
            if (it != ids.end()) id[v] = it->second;
            else {
                int fresh = ids.size();
                id[v] = ids[ch[v]] = fresh;
            }
            if (v != root) ch[par[v]].push_back(id[v]);
        }
        return id;
    }
    int rooted(const vector<vector<int>>& adj, int root) { return rooted_all(adj, root)[root]; }
    int unrooted(const vector<vector<int>>& adj) {  // root at the center (or the better of two centers)
        int n = adj.size();
        vector<int> deg(n), leaves;
        for (int v = 0; v < n; v++) if ((deg[v] = adj[v].size()) <= 1) leaves.push_back(v);
        int left = n;
        while (left > 2) {  // peel leaves layer by layer; the last one or two vertices are the centers
            vector<int> nxt;
            left -= leaves.size();
            for (int v : leaves)
                for (int w : adj[v]) if (--deg[w] == 1) nxt.push_back(w);
            leaves = nxt;
        }
        int best = INT_MAX;
        for (int c : leaves) best = min(best, rooted(adj, c));
        return best;
    }
};
// @end
