// @name   Cartesian tree
// @prefix cpcartesian
// @key    u c
// @desc   [global] CartesianTree ct(a): min-heap on values, in-order = array order (ties: the leftmost is higher). ct.root, ct.par[i], ct.lc[i], ct.rc[i] (-1 = none). The subtree of i is the maximal range where a[i] is the minimum.
// @usage  CartesianTree ct(a);
// @usage  int r = ct.root; for (int i = 0; i < n; i++) { int p = ct.par[i]; }
// @complexity O(n) (one stack pass)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct CartesianTree {
    int root = -1;
    vector<int> par, lc, rc;
    template <class T>
    CartesianTree(const vector<T>& a) {
        int n = a.size();
        par.assign(n, -1), lc.assign(n, -1), rc.assign(n, -1);
        vector<int> st;  // the right spine of the tree built so far, increasing values from bottom to top
        for (int i = 0; i < n; i++) {
            int last = -1;
            while (!st.empty() && a[st.back()] > a[i]) last = st.back(), st.pop_back();
            if (last != -1) lc[i] = last, par[last] = i;          // the popped part becomes i's left subtree
            if (!st.empty()) rc[st.back()] = i, par[i] = st.back();  // i hangs to the right of the new top
            st.push_back(i);
        }
        root = st.empty() ? -1 : st[0];
    }
};
// @end
