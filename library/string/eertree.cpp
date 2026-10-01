// @name   Palindromic tree (eertree)
// @prefix cpeertree
// @key    u shift+e
// @desc   [global] Eertree t; t.add(c) for each char (lowercase). Nodes 2.. are the distinct palindromic substrings (len, link = longest proper palindromic suffix); t.last = longest palindromic suffix of the current prefix. occurrences() counts each palindrome's occurrences.
// @usage  Eertree t; for (char c : s) t.add(c);
// @usage  int distinct = t.t.size() - 2; vector<ll> occ = t.occurrences();
// @complexity O(n * alphabet) memory, O(n) amortized time
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Eertree {
    struct Nd {
        int len, link;
        array<int, 26> nxt;
        ll cnt;  // times this palindrome was the longest palindromic suffix of a prefix
    };
    vector<Nd> t;
    string s;
    int last = 1;
    Eertree() {
        Nd root{-1, 0, {}, 0}, empty{0, 0, {}, 0};  // node 0: imaginary root of length -1; node 1: the empty palindrome
        t = {root, empty};
    }
    int suffix_with(int v, int i) const {  // longest palindromic suffix X of v's chain with s[i - |X| - 1] == s[i]
        while (true) {
            int l = t[v].len;
            if (i - 1 - l >= 0 && s[i - 1 - l] == s[i]) return v;
            v = t[v].link;
        }
    }
    void add(char c) {
        s.push_back(c);
        int i = s.size() - 1, ch = c - 'a', cur = suffix_with(last, i);
        if (!t[cur].nxt[ch]) {  // new palindrome c X c
            int link = 1;
            if (t[cur].len != -1) link = t[suffix_with(t[cur].link, i)].nxt[ch];
            Nd nd{t[cur].len + 2, link, {}, 0};
            t.push_back(nd);
            t[cur].nxt[ch] = t.size() - 1;
        }
        last = t[cur].nxt[ch];
        t[last].cnt++;
    }
    vector<ll> occurrences() const {  // every palindrome also occurs wherever a longer palindrome ending at it occurs
        vector<ll> occ(t.size());
        for (size_t v = 0; v < t.size(); v++) occ[v] = t[v].cnt;
        for (size_t v = t.size() - 1; v >= 2; v--) occ[t[v].link] += occ[v];  // links point to earlier nodes
        return occ;
    }
};
// @end
