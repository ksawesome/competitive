// @name   Intervals: merge + max disjoint (greedy)
// @prefix cpintervals
// @key    t shift+n
// @desc   [global] merge_intervals(v): union of closed [l, r] intervals; max_disjoint(v): most pairwise non-overlapping (earliest end first).
// @usage  vector<pair<ll, ll>> segs = merge_intervals(v);
// @usage  int k = max_disjoint(v);  // touching intervals ([1, 2] and [2, 3]) count as overlapping
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
vector<pair<T, T>> merge_intervals(vector<pair<T, T>> v) {
    sort(v.begin(), v.end());
    vector<pair<T, T>> res;
    for (auto& [l, r] : v) {
        if (!res.empty() && l <= res.back().second) res.back().second = max(res.back().second, r);
        else res.push_back({l, r});
    }
    return res;
}
template <class T>
int max_disjoint(vector<pair<T, T>> v) {
    sort(v.begin(), v.end(), [](const pair<T, T>& a, const pair<T, T>& b) { return a.second < b.second; });
    int cnt = 0;
    bool any = false;
    T last{};
    for (auto& [l, r] : v)
        if (!any || l > last) any = true, last = r, cnt++;
    return cnt;
}
// @end
