// @name   CDQ divide and conquer (3D dominance counting)
// @prefix cpcdq
// @key    u 3
// @desc   [global] cdq_dominance(pts): for each point i, the number of j != i with a_j <= a_i, b_j <= b_i, c_j <= c_i. The pattern (sort by one key, divide and conquer on the second, BIT on the third) solves offline 3D problems.
// @usage  vector<array<ll, 3>> pts(n);   // (a, b, c)
// @usage  vector<int> dom = cdq_dominance(pts);
// @complexity O(n log^2 n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<int> cdq_dominance(const vector<array<ll, 3>>& pts) {
    int n = pts.size();
    vector<int> id(n), res(n, 0);
    iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](int x, int y) { return pts[x] < pts[y]; });
    // group identical points: each one dominates all the others of its group
    struct Item { ll b, c; int cnt, ans; vector<int> who; };
    vector<Item> it;
    for (int i = 0; i < n; i++) {
        if (i && pts[id[i]] == pts[id[i - 1]]) it.back().cnt++, it.back().who.push_back(id[i]);
        else it.push_back({pts[id[i]][1], pts[id[i]][2], 1, 0, {id[i]}});
    }
    vector<ll> cs;
    for (auto& x : it) cs.push_back(x.c);
    sort(cs.begin(), cs.end()), cs.erase(unique(cs.begin(), cs.end()), cs.end());
    vector<int> bit(cs.size() + 1, 0);
    auto add = [&](int pos, int v) { for (pos++; pos <= (int)cs.size(); pos += pos & -pos) bit[pos] += v; };
    auto sum = [&](int pos) { int s = 0; for (pos++; pos > 0; pos -= pos & -pos) s += bit[pos]; return s; };
    auto cidx = [&](ll c) { return (int)(lower_bound(cs.begin(), cs.end(), c) - cs.begin()); };
    vector<int> order(it.size());
    iota(order.begin(), order.end(), 0);  // items are sorted by (a, b, c): left half never has a larger a
    auto solve = [&](auto&& self, int l, int r) -> void {
        if (r - l <= 1) return;
        int m = (l + r) / 2;
        self(self, l, m), self(self, m, r);
        // order[l..m) and order[m..r) are now sorted by (b, c); left items dominate right ones in a
        int i = l;
        for (int j = m; j < r; j++) {
            Item& y = it[order[j]];
            for (; i < m && make_pair(it[order[i]].b, it[order[i]].c) <= make_pair(y.b, y.c) ; i++)
                if (it[order[i]].b <= y.b) add(cidx(it[order[i]].c), it[order[i]].cnt);
            y.ans += sum(cidx(y.c));
        }
        for (int k = l; k < i; k++) add(cidx(it[order[k]].c), -it[order[k]].cnt);
        inplace_merge(order.begin() + l, order.begin() + m, order.begin() + r,
                      [&](int x, int y) { return make_pair(it[x].b, it[x].c) < make_pair(it[y].b, it[y].c); });
    };
    solve(solve, 0, it.size());
    for (auto& x : it)
        for (int w : x.who) res[w] = x.ans + x.cnt - 1;
    return res;
}
// @end
