// @name   Grundy numbers (Sprague-Grundy)
// @prefix cpgrundy
// @key    y g
// @desc   [global] grundy_numbers(N, moves): g[s] = mex of g over moves(s) (moves must go to smaller states). XOR of games != 0 -> first player wins.
// @usage  auto g = grundy_numbers(N, [&](int s) { vector<int> to; for (int k : {1, 3, 4}) if (s >= k) to.push_back(s - k); return to; });
// @usage  int x = 0; for (int pile : piles) x ^= g[pile]; bool first_wins = x != 0;
// @complexity O(N * moves)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class Moves>
vector<int> grundy_numbers(int N, Moves moves) {
    vector<int> g(N + 1);
    for (int s = 0; s <= N; s++) {
        vector<int> seen;
        for (int t : moves(s)) seen.push_back(g[t]);
        sort(seen.begin(), seen.end());
        int mex = 0;
        for (int x : seen)
            if (x == mex) mex++;
            else if (x > mex) break;
        g[s] = mex;
    }
    return g;
}
// @end
