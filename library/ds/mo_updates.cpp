// @name   Mo's algorithm with updates (time as a third dimension)
// @prefix cpmoupdates
// @key    u shift+o
// @desc   [global] mo_with_updates(n, qs, add, remove, apply, answer): qs[i] = {l, r, t} asks about [l, r) after the first t updates. apply(u, L, R) toggles update u (swap in/out) given the window [L, R).
// @usage  mo_with_updates(n, qs, add, remove, [&](int u, int L, int R) { auto& [p, v] = upd[u]; if (L <= p && p < R) remove(p); swap(a[p], v); if (L <= p && p < R) add(p); }, [&](int i) { ans[i] = cur; });
// @complexity O(n^(5/3)) for n queries and updates
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class Add, class Remove, class Apply, class Answer>
void mo_with_updates(int n, const vector<array<int, 3>>& qs, Add add, Remove remove, Apply apply, Answer answer) {
    int q = qs.size();
    int block = max<int>(1, cbrt((double)n * n));  // n^(2/3)
    vector<int> order(q);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int x, int y) {
        const auto &a = qs[x], &b = qs[y];
        if (a[0] / block != b[0] / block) return a[0] < b[0];
        if (a[1] / block != b[1] / block) return a[1] < b[1];
        return a[2] < b[2];
    });
    int L = 0, R = 0, T = 0;  // window [L, R), the first T updates applied
    for (int i : order) {
        auto [l, r, t] = qs[i];
        while (T < t) apply(T++, L, R);
        while (T > t) apply(--T, L, R);
        while (L > l) add(--L);
        while (R < r) add(R++);
        while (L < l) remove(L++);
        while (R > r) remove(--R);
        answer(i);
    }
}
// @end
