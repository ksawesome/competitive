// @name   Bidirectional BFS (meet in the middle on shortest paths)
// @prefix cpbibfs
// @key    u 2
// @desc   [global] bidirectional_bfs(s, t, neighbours): unweighted distance from s to t (-1 if unreachable) exploring from both ends; neighbours(state) returns a vector of states. Works on implicit graphs (puzzles, strings) with any hashable State.
// @usage  ll d = bidirectional_bfs<ll>(start, goal, [&](ll s) { vector<ll> nb; /* moves */ return nb; });
// @complexity O(b^(d/2)) states for branching factor b and distance d, instead of O(b^d)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class State, class Nb>
ll bidirectional_bfs(const State& s, const State& t, Nb neighbours) {
    if (s == t) return 0;
    unordered_map<State, ll> ds{{s, 0}}, dt{{t, 0}};
    vector<State> fs{s}, ft{t};  // current frontiers
    while (!fs.empty() && !ft.empty()) {
        bool from_s = fs.size() <= ft.size();  // always expand the smaller frontier by one full layer
        auto& f = from_s ? fs : ft;
        auto& mine = from_s ? ds : dt;
        auto& other = from_s ? dt : ds;
        vector<State> nxt;
        ll best = -1;
        for (const State& u : f)
            for (const State& v : neighbours(u)) {
                if (mine.count(v)) continue;
                mine[v] = mine[u] + 1;
                auto it = other.find(v);
                if (it != other.end() && (best < 0 || mine[v] + it->second < best)) best = mine[v] + it->second;
                nxt.push_back(v);
            }
        if (best >= 0) return best;  // the whole layer was expanded, so best is the minimum
        f = move(nxt);
    }
    return -1;
}
// @end
