// @name   Segment tree with custom nodes + binary search
// @prefix cpsegnode
// @key    y n
// @desc   [global] SegNode st(n or leaves): any associative Node (example: max subarray sum); st.max_right(l, pred) walks down in O(log n).
// @usage  SegNode st(n); st.set(i, Node::leaf(x)); Node r = st.query(l, r); r.best;
// @usage  int r = st.max_right(l, [&](const Node& x) { return x.sum < k; });  // first r with sum of [l, r] >= k is r
// @complexity O(log n) per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Node {  // edit the fields and operator+ (must be associative; the default Node is the identity)
    static constexpr ll NEG = LLONG_MIN / 4;
    ll sum = 0, pref = NEG, suf = NEG, best = NEG;
    static Node leaf(ll x) { return {x, x, x, x}; }
    friend Node operator+(const Node& a, const Node& b) {
        return {a.sum + b.sum, max(a.pref, a.sum + b.pref), max(b.suf, b.sum + a.suf),
                max({a.best, b.best, a.suf + b.pref})};
    }
};
struct SegNode {
    int n, size = 1;
    vector<Node> t;
    SegNode(int n_) : n(n_) {
        while (size < n) size <<= 1;
        t.assign(2 * size, Node());
    }
    SegNode(const vector<Node>& leaves) : SegNode((int)leaves.size()) {
        for (int i = 0; i < n; i++) t[size + i] = leaves[i];
        for (int i = size - 1; i > 0; i--) t[i] = t[2 * i] + t[2 * i + 1];
    }
    void set(int i, const Node& v) {
        for (t[i += size] = v; i >>= 1;) t[i] = t[2 * i] + t[2 * i + 1];
    }
    Node get(int i) const { return t[size + i]; }
    Node query(int l, int r) const {  // [l, r)
        Node L, R;
        for (l += size, r += size; l < r; l >>= 1, r >>= 1) {
            if (l & 1) L = L + t[l++];
            if (r & 1) R = t[--r] + R;
        }
        return L + R;
    }
    template <class P>
    int max_right(int l, P pred) const {  // largest r with pred(query(l, r)) true; pred(Node()) must be true
        if (l == n) return n;
        l += size;
        Node acc;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!pred(acc + t[l])) {
                while (l < size) {
                    l = 2 * l;
                    if (pred(acc + t[l])) acc = acc + t[l], l++;
                }
                return l - size;
            }
            acc = acc + t[l], l++;
        } while ((l & -l) != l);
        return n;
    }
};
// @end
