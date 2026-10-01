// @name   Mo's algorithm (offline range queries)
// @prefix cpmo
// @key    y o
// @desc   [global] mo(n, queries, add, remove, answer): queries [l, r); add(i) / remove(i) update the window, answer(qi) records query qi.
// @usage  vector<int> cnt(maxv), res(q); int distinct = 0;
// @usage  mo(n, qs, [&](int i) { distinct += cnt[a[i]]++ == 0; }, [&](int i) { distinct -= --cnt[a[i]] == 0; }, [&](int qi) { res[qi] = distinct; });
// @complexity O((n + q) sqrt n) add/remove calls
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class Add, class Remove, class Answer>
void mo(int n, const vector<pair<int, int>>& queries, Add add, Remove remove, Answer answer) {
    int q = queries.size();
    int block = max<int>(1, (int)(n / max(1.0, sqrt((double)q))));
    vector<int> order(q);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        int ba = queries[a].first / block, bb = queries[b].first / block;
        if (ba != bb) return ba < bb;
        return (ba & 1) ? queries[a].second > queries[b].second : queries[a].second < queries[b].second;
    });
    int L = 0, R = 0;  // current window [L, R)
    for (int qi : order) {
        auto [l, r] = queries[qi];
        while (R < r) add(R++);
        while (L > l) add(--L);
        while (R > r) remove(--R);
        while (L < l) remove(L++);
        answer(qi);
    }
}
// @end
