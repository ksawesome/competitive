// @name   Implicit treap (sequence with split / merge)
// @prefix cptreap
// @key    y t
// @desc   [global] Treap tr; tr.insert(pos, v), tr.erase(pos), tr.reverse_range(l, r), tr.range_sum(l, r), tr.to_vector(). Expected O(log n).
// @usage  Treap tr; for (auto x : a) tr.insert(tr.count(), x);
// @usage  tr.reverse_range(l, r); ll s = tr.range_sum(l, r); vector<ll> now = tr.to_vector();
// @complexity O(log n) expected per operation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Treap {
    struct Nd {
        ll val, sum;
        unsigned pri;
        int size, l, r;
        bool rev;
    };
    vector<Nd> t{{0, 0, 0, 0, 0, 0, false}};  // node 0 = empty
    int root = 0;
    mt19937 rng{(unsigned)chrono::steady_clock::now().time_since_epoch().count()};
    int count() const { return t[root].size; }
    void insert(int pos, ll v) {
        t.push_back({v, v, (unsigned)rng(), 1, 0, 0, false});
        int nd = t.size() - 1;
        auto [a, b] = split(root, pos);
        root = merge(merge(a, nd), b);
    }
    void erase(int pos) {
        auto [a, bc] = split(root, pos);
        auto [b, c] = split(bc, 1);
        (void)b;
        root = merge(a, c);
    }
    void reverse_range(int l, int r) {  // [l, r)
        auto [a, bc] = split(root, l);
        auto [b, c] = split(bc, r - l);
        if (b) t[b].rev ^= 1;
        root = merge(merge(a, b), c);
    }
    ll range_sum(int l, int r) {  // [l, r)
        auto [a, bc] = split(root, l);
        auto [b, c] = split(bc, r - l);
        ll s = t[b].sum;
        root = merge(merge(a, b), c);
        return s;
    }
    vector<ll> to_vector() {
        vector<ll> out;
        auto walk = [&](auto&& self, int x) -> void {
            if (!x) return;
            push(x);
            self(self, t[x].l), out.push_back(t[x].val), self(self, t[x].r);
        };
        walk(walk, root);
        return out;
    }

   private:
    void pull(int x) {
        t[x].size = 1 + t[t[x].l].size + t[t[x].r].size;
        t[x].sum = t[x].val + t[t[x].l].sum + t[t[x].r].sum;
    }
    void push(int x) {
        if (!t[x].rev) return;
        swap(t[x].l, t[x].r);
        if (t[x].l) t[t[x].l].rev ^= 1;
        if (t[x].r) t[t[x].r].rev ^= 1;
        t[x].rev = false;
    }
    pair<int, int> split(int x, int k) {  // first k elements | the rest
        if (!x) return {0, 0};
        push(x);
        if (t[t[x].l].size >= k) {
            auto [a, b] = split(t[x].l, k);
            t[x].l = b, pull(x);
            return {a, x};
        }
        auto [a, b] = split(t[x].r, k - t[t[x].l].size - 1);
        t[x].r = a, pull(x);
        return {x, b};
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (t[a].pri > t[b].pri) {
            push(a);
            t[a].r = merge(t[a].r, b), pull(a);
            return a;
        }
        push(b);
        t[b].l = merge(a, t[b].l), pull(b);
        return b;
    }
};
// @end
