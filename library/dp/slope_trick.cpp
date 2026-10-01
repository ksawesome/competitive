// @name   Slope trick (make an array non-decreasing)
// @prefix cpslope
// @key    y shift+s
// @desc   [global] make_non_decreasing(a): min total |a[i] - b[i]| over non-decreasing b. Max-heap of slope breakpoints. Strictly increasing: use a[i] - i.
// @usage  ll cost = make_non_decreasing(a);
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class T>
ll make_non_decreasing(const vector<T>& a) {
    priority_queue<ll> pq;  // breakpoints of the convex cost function f(x) = best cost with last value x
    ll cost = 0;
    for (auto x : a) {
        pq.push(x);
        if (pq.top() > x) {
            cost += pq.top() - x;
            pq.pop();
            pq.push(x);
        }
    }
    return cost;
}
// @end
