// @name   Persistent segment tree (k-th smallest in range)
// @prefix cppersist
// @key    y p
// @desc   [global] PersistentSeg ps(n): root = ps.build(a) in O(n) nodes; root2 = ps.update(root1, pos, v) keeps every version; ps.query(root, l, r); ps.kth(rootL, rootR, k).
// @usage  vector<int> root{0}; for (int i = 0; i < n; i++) root.push_back(ps.update(root[i], rank[i], 1));  // ranks from cpcompress
// @usage  int idx = ps.kth(root[l], root[r], k);  // k-th smallest (0-based) of a[l, r) is vals[idx]
// @usage  int r0 = ps.build(a);  // a version holding the values a, instead of n updates (n log n nodes)
// @complexity O(log n) per operation and O(log n) new nodes per update
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct PersistentSeg {
    struct Nd {
        ll sum;
        int l, r;
    };
    int n;
    vector<Nd> t{{0, 0, 0}};  // node 0 = the empty tree (its children are itself)
    PersistentSeg(int n_) : n(n_) {}
    template <class U>
    int build(const vector<U>& a) { return n ? build(a, 0, n) : 0; }  // a version holding the values a: 2n nodes
    int update(int root, int pos, ll v) { return update(root, 0, n, pos, v); }  // new root with a[pos] += v
    ll query(int root, int l, int r) const { return query(root, 0, n, l, r); }
    int kth(int ra, int rb, ll k) const {  // position of the k-th unit (0-based) in version rb minus version ra
        int a = 0, b = n;
        while (b - a > 1) {
            int mid = (a + b) / 2;
            ll left = t[t[rb].l].sum - t[t[ra].l].sum;
            if (k < left) ra = t[ra].l, rb = t[rb].l, b = mid;
            else k -= left, ra = t[ra].r, rb = t[rb].r, a = mid;
        }
        return a;
    }

   private:
    template <class U>
    int build(const vector<U>& v, int a, int b) {
        if (b - a == 1) {
            t.push_back({(ll)v[a], 0, 0});
            return t.size() - 1;
        }
        int mid = (a + b) / 2;
        int l = build(v, a, mid), r = build(v, mid, b);
        t.push_back({t[l].sum + t[r].sum, l, r});
        return t.size() - 1;
    }
    int update(int x, int a, int b, int pos, ll v) {
        Nd copy = t[x];
        copy.sum += v;
        if (b - a > 1) {
            int mid = (a + b) / 2;
            if (pos < mid) copy.l = update(t[x].l, a, mid, pos, v);
            else copy.r = update(t[x].r, mid, b, pos, v);
        }
        t.push_back(copy);
        return t.size() - 1;
    }
    ll query(int x, int a, int b, int l, int r) const {
        if (!x || r <= a || b <= l) return 0;
        if (l <= a && b <= r) return t[x].sum;
        int mid = (a + b) / 2;
        return query(t[x].l, a, mid, l, r) + query(t[x].r, mid, b, l, r);
    }
};
// @end
