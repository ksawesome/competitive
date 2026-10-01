// @name   All permutations (next_permutation)
// @prefix cpperm
// @key    t 9
// @desc   [inside solve] needs n (n <= ~10). Visits every order of 0..n-1 in lexicographic order.
// @usage  // on a sorted vector v directly: do { ... } while (next_permutation(v.begin(), v.end()));  (skips duplicates)
// @complexity O(n! n)
#include <bits/stdc++.h>
using namespace std;

void context(int n) {
    // @begin
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    do {
        // use perm here
    } while (next_permutation(perm.begin(), perm.end()));
    // @end
}
