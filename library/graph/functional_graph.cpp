// @name   Functional graph (successor graph) cycles
// @prefix cpfuncgraph
// @key    t ,
// @desc   [global] FunctionalGraph fg(f), f[v] = the one successor of v: fg.cyc[v] (cycle id or -1), fg.cyc_len[id], fg.dist[v] to its cycle, fg.entry[v].
// @usage  FunctionalGraph fg(f);
// @usage  int steps_until_repeat = fg.dist[v] + fg.cyc_len[fg.cyc[fg.entry[v]]];
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

// @begin
struct FunctionalGraph {
    vector<int> cyc, cyc_len, dist, entry;  // entry[v] = first cycle vertex reached from v
    FunctionalGraph(const vector<int>& f) : cyc(f.size(), -1), dist(f.size(), -1), entry(f.size(), -1) {
        int n = f.size();
        vector<int> state(n);  // 0 unseen, 1 on the current walk, 2 done
        for (int s = 0; s < n; s++) {
            if (state[s]) continue;
            vector<int> path;
            int v = s;
            while (!state[v]) state[v] = 1, path.push_back(v), v = f[v];
            for (int u : path) state[u] = 2;
            if (find(path.begin(), path.end(), v) != path.end()) {  // the walk closed a new cycle at v
                auto it = find(path.begin(), path.end(), v);
                int id = cyc_len.size();
                cyc_len.push_back(path.end() - it);
                for (auto jt = it; jt != path.end(); jt++) cyc[*jt] = id, dist[*jt] = 0, entry[*jt] = *jt;
                path.erase(it, path.end());
            }
            for (int i = (int)path.size() - 1; i >= 0; i--) {  // tail vertices lead into known ones
                int u = path[i];
                dist[u] = dist[f[u]] + 1;
                entry[u] = entry[f[u]];
            }
        }
    }
};
// @end
