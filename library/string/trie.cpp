// @name   Trie
// @prefix cptrie
// @key    t shift+t
// @desc   [global] Trie t; t.insert(s); t.count(s) = copies of s; t.prefix_count(p) = words starting with p. a-z only.
// @usage  Trie t;
// @usage  t.insert(s); t.count(s); t.prefix_count(p);
// @complexity O(|s|) per operation
#include <bits/stdc++.h>
using namespace std;

// @begin
struct Trie {
    vector<array<int, 26>> nxt;  // 0 = no child (the root is never a child)
    vector<int> ends, pass;
    Trie() : nxt(1), ends(1), pass(1) {}
    void insert(const string& s) {
        int u = 0;
        pass[0]++;
        for (char ch : s) {
            int c = ch - 'a';
            if (!nxt[u][c]) {
                nxt[u][c] = nxt.size();
                nxt.emplace_back();
                ends.push_back(0);
                pass.push_back(0);
            }
            u = nxt[u][c];
            pass[u]++;
        }
        ends[u]++;
    }
    int walk(const string& s) const {  // node for s, -1 if absent
        int u = 0;
        for (char ch : s)
            if (!(u = nxt[u][ch - 'a'])) return -1;
        return u;
    }
    int count(const string& s) const { int u = walk(s); return u < 0 ? 0 : ends[u]; }
    int prefix_count(const string& p) const { int u = walk(p); return u < 0 ? 0 : pass[u]; }
};
// @end
