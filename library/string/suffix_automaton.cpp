// @name   Suffix automaton
// @prefix cpsam
// @key    y 5
// @desc   [global] SuffixAutomaton sam(s): smallest automaton of all substrings. sam.count_distinct(), sam.contains(t); states hold len / link / next.
// @usage  SuffixAutomaton sam(s);
// @usage  ll d = sam.count_distinct(); bool in = sam.contains(t);
// @complexity O(n * 26)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct SuffixAutomaton {
    struct State {
        int len, link;
        array<int, 26> next;
    };
    vector<State> st;
    int last = 0;
    SuffixAutomaton(const string& s) {
        st.reserve(2 * s.size() + 2);
        st.push_back({0, -1, {}});
        st[0].next.fill(-1);
        for (char ch : s) extend(ch - 'a');
    }
    void extend(int c) {
        int cur = st.size();
        st.push_back({st[last].len + 1, 0, {}});
        st[cur].next.fill(-1);
        int p = last;
        while (p != -1 && st[p].next[c] == -1) st[p].next[c] = cur, p = st[p].link;
        if (p != -1) {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) st[cur].link = q;
            else {
                State copy = st[q];
                copy.len = st[p].len + 1;
                int clone = st.size();
                st.push_back(copy);
                while (p != -1 && st[p].next[c] == q) st[p].next[c] = clone, p = st[p].link;
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }
    ll count_distinct() const {  // number of distinct non-empty substrings
        ll r = 0;
        for (size_t i = 1; i < st.size(); i++) r += st[i].len - st[st[i].link].len;
        return r;
    }
    bool contains(const string& t) const {
        int u = 0;
        for (char ch : t)
            if ((u = st[u].next[ch - 'a']) == -1) return false;
        return true;
    }
};
// @end
