// @name   Monotonic stack (next greater / previous smaller)
// @prefix cpmonostack
// @key    t 7
// @desc   [global] next_greater(a)[i] = first j > i with a[j] > a[i] (n if none); prev_smaller(a)[i] = last j < i with a[j] < a[i] (-1).
// @usage  auto nxt = next_greater(a); auto prv = prev_smaller(a);
// @usage  // largest rectangle in a histogram: width of bar i = next smaller - prev smaller - 1
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
vector<int> next_greater(const vector<T>& a) {
    int n = a.size();
    vector<int> res(n, n), st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && a[st.back()] < a[i]) res[st.back()] = i, st.pop_back();
        st.push_back(i);
    }
    return res;
}
template <class T>
vector<int> prev_smaller(const vector<T>& a) {
    int n = a.size();
    vector<int> res(n, -1), st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        if (!st.empty()) res[i] = st.back();
        st.push_back(i);
    }
    return res;
}
// @end
