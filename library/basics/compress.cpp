// @name   Coordinate compression
// @prefix cpcompress
// @key    t shift+c
// @desc   [global] auto vals = compress(a); a[i] becomes its rank 0..k-1 and vals[rank] is the original value.
// @usage  vector<int> vals = compress(a);  // a[i] -> rank, vals[rank] -> value
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
vector<T> compress(vector<T>& a) {
    vector<T> vals = a;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    for (auto& x : a) x = lower_bound(vals.begin(), vals.end(), x) - vals.begin();
    return vals;
}
// @end
