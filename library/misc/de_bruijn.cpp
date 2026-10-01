// @name   De Bruijn sequence
// @prefix cpdebruijn
// @key    u 9
// @desc   [global] de_bruijn(k, n): a cyclic sequence of length k^n over {0..k-1} in which every length-n word appears exactly once (read cyclically). Built from Lyndon words (FKM algorithm). Shortest string containing all words: the sequence + its first n-1 symbols.
// @usage  vector<int> seq = de_bruijn(2, 3);   // 0 0 0 1 0 1 1 1
// @complexity O(k^n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<int> de_bruijn(int k, int n) {
    vector<int> a(n + 1, 0), seq;
    auto gen = [&](auto&& self, int t, int p) -> void {  // concatenating the Lyndon words whose length divides n, in order
        if (t > n) {
            if (n % p == 0)
                for (int j = 1; j <= p; j++) seq.push_back(a[j]);
            return;
        }
        a[t] = a[t - p];
        self(self, t + 1, p);
        for (int j = a[t - p] + 1; j < k; j++) a[t] = j, self(self, t + 1, t);
    };
    gen(gen, 1, 1);
    return seq;
}
// @end
