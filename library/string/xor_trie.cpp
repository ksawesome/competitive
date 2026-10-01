// @name   Binary trie (maximum XOR)
// @prefix cpxortrie
// @key    t `
// @desc   [global] XorTrie t; t.insert(x), t.erase(x) (x must be present), t.max_xor(x) = max over stored y of x ^ y (non-empty). 30 bits.
// @usage  XorTrie t; t.insert(0);  // prefix XORs: best subarray XOR = max over i of t.max_xor(pref[i])
// @usage  ll best = t.max_xor(x);
// @complexity O(B) per operation, B = 30 (set B = 61 for 64-bit values)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct XorTrie {
    static const int B = 30;
    vector<array<int, 2>> nxt;
    vector<int> cnt;
    XorTrie() : nxt(1), cnt(1) {}
    void insert(ll x, int d = 1) {
        int u = 0;
        cnt[0] += d;
        for (int i = B - 1; i >= 0; i--) {
            int b = x >> i & 1;
            if (!nxt[u][b]) nxt[u][b] = nxt.size(), nxt.push_back({0, 0}), cnt.push_back(0);
            u = nxt[u][b], cnt[u] += d;
        }
    }
    void erase(ll x) { insert(x, -1); }
    ll max_xor(ll x) const {
        int u = 0;
        ll r = 0;
        for (int i = B - 1; i >= 0; i--) {
            int b = x >> i & 1, want = nxt[u][b ^ 1];
            if (want && cnt[want] > 0) r |= 1LL << i, u = want;
            else u = nxt[u][b];
        }
        return r;
    }
};
// @end
