// @name   Aho-Corasick (many patterns at once)
// @prefix cpaho
// @key    y shift+a
// @desc   [global] AhoCorasick ac; id = ac.add(p) per pattern; ac.build(); ac.occurrences(text)[id] = matches of that pattern. a-z only.
// @usage  AhoCorasick ac; vector<int> id; for (auto& p : pats) id.push_back(ac.add(p)); ac.build();
// @usage  auto occ = ac.occurrences(text); ll hits = occ[id[i]];  // or walk: s = ac.go(s, ch), ac.out[s] = matches ending here
// @complexity O(total pattern length * 26 + |text|)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct AhoCorasick {
    vector<array<int, 26>> nxt;
    vector<int> link, out, bfs_order;  // out[s] = patterns ending at s or at any suffix-link ancestor
    AhoCorasick() : nxt(1), link(1, 0), out(1, 0) { nxt[0].fill(-1); }
    int add(const string& s) {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (nxt[u][c] == -1) {
                nxt[u][c] = nxt.size();
                nxt.emplace_back(), nxt.back().fill(-1), link.push_back(0), out.push_back(0);
            }
            u = nxt[u][c];
        }
        out[u]++;
        return u;
    }
    void build() {  // BFS: suffix links + full transition table
        queue<int> q;
        for (int c = 0; c < 26; c++) {
            if (nxt[0][c] == -1) nxt[0][c] = 0;
            else link[nxt[0][c]] = 0, q.push(nxt[0][c]);
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            bfs_order.push_back(u);
            out[u] += out[link[u]];
            for (int c = 0; c < 26; c++) {
                int v = nxt[u][c];
                if (v == -1) nxt[u][c] = nxt[link[u]][c];
                else link[v] = nxt[link[u]][c], q.push(v);
            }
        }
    }
    int go(int s, char ch) const { return nxt[s][ch - 'a']; }
    vector<ll> occurrences(const string& text) const {  // occ[s] = times the string of state s occurs in text
        vector<ll> occ(nxt.size());
        int s = 0;
        for (char ch : text) s = go(s, ch), occ[s]++;
        for (int i = (int)bfs_order.size() - 1; i >= 0; i--) occ[link[bfs_order[i]]] += occ[bfs_order[i]];
        return occ;
    }
};
// @end
