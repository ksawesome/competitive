// @name   Random hashing of sets and multisets (XOR / sum hashing)
// @prefix cpxorhash
// @key    u x
// @desc   [global] RandHash h; h(x) = a random 64-bit label for value x. Multisets are equal (whp) iff the sums of labels match; "every value appears an even number of times" iff the XOR of labels is 0. With prefix sums / XORs this answers range questions in O(1).
// @usage  RandHash h; vector<uint64_t> px(n + 1); for (int i = 0; i < n; i++) px[i + 1] = px[i] ^ h(a[i]);
// @usage  bool all_even = (px[r] ^ px[l]) == 0;   // range [l, r)
// @complexity O(1) per label
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct RandHash {
    uint64_t seed = chrono::steady_clock::now().time_since_epoch().count() * 0x9e3779b97f4a7c15ULL;
    uint64_t operator()(uint64_t x) const {  // splitmix64 of (x + seed): labels look independent and uniform
        x += seed + 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
};
// @end
