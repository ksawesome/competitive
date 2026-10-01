// @name   Read vector
// @prefix cpread
// @key    t v
// @desc   [inside solve] needs n. Reads n numbers into a.
// @usage  // with the template: vi a(n); cin >> a;  does the same
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

void context(int n) {
    // @begin
    vector<int> a(n);
    for (auto& x : a) cin >> x;
    // @end
}
