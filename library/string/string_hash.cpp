// @name   String hashing (mod 2^61-1)
// @prefix cpstrhash
// @key    t h
// @desc   [global] StrHash h(s); h.get(l, r) = hash of s[l, r). Random base, hack-safe; same base for every string.
// @usage  StrHash h(s);
// @usage  if (h.get(l1, r1) == h.get(l2, r2)) { /* s[l1, r1) == s[l2, r2) */ }
// @complexity O(n) build, O(1) per substring
#include <bits/stdc++.h>
using namespace std;

// @begin
struct StrHash {
    static constexpr uint64_t MODH = (1ULL << 61) - 1;
    static uint64_t mul(uint64_t a, uint64_t b) {  // a * b mod 2^61-1 via 32-bit halves (no __int128)
        uint64_t a1 = a >> 32, a0 = a & 0xFFFFFFFF, b1 = b >> 32, b0 = b & 0xFFFFFFFF;
        uint64_t mid = a1 * b0 + a0 * b1, lo = a0 * b0;
        uint64_t s = (a1 * b1 << 3) + (mid >> 29) + ((mid & ((1ULL << 29) - 1)) << 32) + (lo >> 61) + (lo & MODH);
        s = (s & MODH) + (s >> 61);
        return s >= MODH ? s - MODH : s;
    }
    static uint64_t base() {
        static const uint64_t b = mt19937_64(chrono::steady_clock::now().time_since_epoch().count())() % (MODH - 1000) + 500;
        return b;
    }
    vector<uint64_t> h, pw;
    StrHash(const string& s) : h(s.size() + 1), pw(s.size() + 1, 1) {
        for (size_t i = 0; i < s.size(); i++) {
            h[i + 1] = (mul(h[i], base()) + (unsigned char)s[i]) % MODH;
            pw[i + 1] = mul(pw[i], base());
        }
    }
    uint64_t get(int l, int r) const {  // [l, r)
        return (h[r] + MODH - mul(h[l], pw[r - l])) % MODH;
    }
};
// @end
