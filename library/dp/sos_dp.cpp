// @name   Sum over subsets (SOS DP)
// @prefix cpsos
// @key    y u
// @desc   [global] sos_subsets(f, B): f[mask] becomes the sum of f[sub] over all submasks; sos_supersets: over supermasks. In place, size 2^B.
// @usage  vector<ll> f(1 << B); /* f[mask] = count of numbers equal to mask */ sos_subsets(f, B);
// @usage  // f[mask] now counts numbers x with (x & mask) == x
// @complexity O(B 2^B)
#include <bits/stdc++.h>
using namespace std;

// @begin
template <class T>
void sos_subsets(vector<T>& f, int B) {
    for (int i = 0; i < B; i++)
        for (int mask = 0; mask < (1 << B); mask++)
            if (mask >> i & 1) f[mask] += f[mask ^ (1 << i)];
}
template <class T>
void sos_supersets(vector<T>& f, int B) {
    for (int i = 0; i < B; i++)
        for (int mask = 0; mask < (1 << B); mask++)
            if (!(mask >> i & 1)) f[mask] += f[mask | (1 << i)];
}
// @end
