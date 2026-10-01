// @name   Longest increasing subsequence
// @prefix cplis
// @key    t shift+l
// @desc   [global] vector<int> idx = lis(a): indices of one strictly increasing LIS, O(n log n). Length = idx.size().
// @usage  vector<int> idx = lis(a);  // idx.size() = LIS length
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
vector<int> lis(const vector<T>& a) {
    vector<int> tail, prev(a.size(), -1);  // tail[k] = index ending the best length-(k+1) run
    for (int i = 0; i < (int)a.size(); i++) {
        // for non-decreasing: compare with <= instead of <
        int k = lower_bound(tail.begin(), tail.end(), i, [&](int j, int cur) { return a[j] < a[cur]; }) - tail.begin();
        if (k > 0) prev[i] = tail[k - 1];
        if (k == (int)tail.size()) tail.push_back(i);
        else tail[k] = i;
    }
    vector<int> res;
    for (int i = tail.empty() ? -1 : tail.back(); i != -1; i = prev[i]) res.push_back(i);
    reverse(res.begin(), res.end());
    return res;
}
// @end
