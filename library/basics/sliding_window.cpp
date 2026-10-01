// @name   Sliding window min / max (monotonic deque)
// @prefix cpwindow
// @key    t 6
// @desc   [global] window_min(a, k)[i] = min of a[i, i + k); pass want_max = true for the max. Monotonic deque.
// @usage  vector<ll> mins = window_min(a, k);
// @usage  vector<ll> maxs = window_min(a, k, true);
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
vector<T> window_min(const vector<T>& a, int k, bool want_max = false) {
    vector<T> res;
    deque<int> dq;  // indices whose values are increasing (decreasing for max) front to back
    for (int i = 0; i < (int)a.size(); i++) {
        while (!dq.empty() && (want_max ? a[dq.back()] <= a[i] : a[dq.back()] >= a[i])) dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - k) dq.pop_front();
        if (i >= k - 1) res.push_back(a[dq.front()]);
    }
    return res;
}
// @end
