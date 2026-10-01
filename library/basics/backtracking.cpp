// @name   Recursive complete search (backtracking)
// @prefix cpbacktrack
// @key    t shift+y
// @desc   [inside solve] needs n. Builds every subsequence of 0..n-1 in cur (skip / take); prune at the top of search().
// @usage  // permutations instead: loop over unused items, mark used, recurse, unmark
// @complexity O(2^n) calls without pruning
#include <bits/stdc++.h>
using namespace std;

void context(int n) {
    // @begin
    vector<int> cur;
    auto search = [&](auto&& self, int i) -> void {
        if (i == n) {
            // process cur
            return;
        }
        self(self, i + 1);  // skip item i
        cur.push_back(i);   // take item i
        self(self, i + 1);
        cur.pop_back();
    };
    search(search, 0);
    // @end
}
