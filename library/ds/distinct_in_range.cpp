// @name   Distinct values in ranges (offline sweep + BIT)
// @prefix cpdistinct
// @key    y shift+d
// @desc   [global] distinct_in_ranges(a, queries): number of distinct values in a[l, r) per query. Sweeps r, keeps only last occurrences.
// @usage  vector<int> res = distinct_in_ranges(a, qs);  // qs[i] = {l, r}
// @complexity O((n + q) log n)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
vector<int> distinct_in_ranges(const vector<T>& a, const vector<pair<int, int>>& queries) {
    int n = a.size(), q = queries.size();
    vector<int> order(q), res(q), bit(n + 1);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int x, int y) { return queries[x].second < queries[y].second; });
    auto add = [&](int i, int v) {
        for (i++; i <= n; i += i & -i) bit[i] += v;
    };
    auto prefix = [&](int i) {
        int s = 0;
        for (; i > 0; i -= i & -i) s += bit[i];
        return s;
    };
    map<T, int> last;
    int r = 0;
    for (int qi : order) {
        auto [l, qr] = queries[qi];
        for (; r < qr; r++) {
            auto it = last.find(a[r]);
            if (it != last.end()) add(it->second, -1);
            last[a[r]] = r;
            add(r, 1);
        }
        res[qi] = prefix(qr) - prefix(l);
    }
    return res;
}
// @end
