// @name   Gray code
// @prefix cpgray
// @key    u 0
// @desc   [global] gray(i) = i ^ (i >> 1): consecutive values differ in exactly one bit; gray_inverse(g) gives i back. To visit all 2^n subsets changing one element at a time, step k flips bit __builtin_ctzll(k).
// @usage  for (ll k = 1, mask = 0; k < (1LL << n); k++) { int b = __builtin_ctzll(k); mask ^= 1LL << b; /* add or remove b */ }
// @usage  ll g = gray(i), back = gray_inverse(g);
// @complexity O(1) / O(log) per value
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll gray(ll i) { return i ^ (i >> 1); }
ll gray_inverse(ll g) {  // prefix XOR of the bits from the top
    ll i = 0;
    for (; g; g >>= 1) i ^= g;
    return i;
}
// @end
