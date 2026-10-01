// @name   Two pointers (sliding window)
// @prefix cptwoptr
// @key    t 5
// @desc   [inside solve] needs n, a (values >= 0). Longest window with sum <= S; grow r, shrink l while the window is invalid.
// @usage  // count windows instead: add (r - l + 1) to an answer inside the loop
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n, vector<int>& a, ll S) {
    // @begin
    int best = 0;  // longest window a[l..r] with sum <= S
    ll cur = 0;
    for (int l = 0, r = 0; r < n; r++) {
        cur += a[r];
        while (cur > S) cur -= a[l++];
        best = max(best, r - l + 1);
    }
    // @end
}
