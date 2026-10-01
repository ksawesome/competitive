// @name   Argsort + custom comparator
// @prefix cpargsort
// @key    t shift+j
// @desc   [global] argsort(a): indices sorted by a[i] (stable). Custom orders: sort with a lambda (see usage).
// @usage  vector<int> idx = argsort(a);  // a[idx[0]] is the smallest
// @usage  sort(v.begin(), v.end(), [](const auto& x, const auto& y) { return make_pair(x.first, -x.second) < make_pair(y.first, -y.second); });
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
vector<int> argsort(const vector<T>& a) {
    vector<int> idx(a.size());
    iota(idx.begin(), idx.end(), 0);
    stable_sort(idx.begin(), idx.end(), [&](int i, int j) { return a[i] < a[j]; });
    return idx;
}
// @end
