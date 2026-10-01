// @name   All subsets (bitmask loop)
// @prefix cpsubsets
// @key    t 8
// @desc   [inside solve] needs n, a (n <= ~20). Visits every subset; bit i of mask set = item i taken.
// @usage  // for all submasks of a mask: for (int s = mask; s; s = (s - 1) & mask)
// @complexity O(2^n n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void context(int n, vector<int>& a) {
    // @begin
    for (int mask = 0; mask < (1 << n); mask++) {
        ll sum = 0;
        for (int i = 0; i < n; i++)
            if (mask >> i & 1) sum += a[i];
        // use the subset here
    }
    // @end
}
