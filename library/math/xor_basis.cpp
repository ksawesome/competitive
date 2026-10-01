// @name   XOR basis (linear basis over GF(2))
// @prefix cpxorbasis
// @key    y shift+x
// @desc   [global] XorBasis xb; xb.insert(x) (false if x was already representable); xb.can_make(x); xb.max_xor(); 2^xb.dim distinct subset XORs.
// @usage  XorBasis xb; for (auto x : a) xb.insert(x);
// @usage  ll best = xb.max_xor(); bool ok = xb.can_make(target);
// @complexity O(B) per operation, B = 62 bits
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct XorBasis {
    static const int B = 62;
    ll b[B] = {};  // b[i] has highest bit i (or is 0)
    int dim = 0;
    bool insert(ll x) {
        for (int i = B - 1; i >= 0; i--) {
            if (!(x >> i & 1)) continue;
            if (!b[i]) return b[i] = x, dim++, true;
            x ^= b[i];
        }
        return false;
    }
    bool can_make(ll x) const {
        for (int i = B - 1; i >= 0; i--)
            if (x >> i & 1) {
                if (!b[i]) return false;
                x ^= b[i];
            }
        return true;
    }
    ll max_xor(ll x = 0) const {  // max of x ^ (some subset XOR)
        for (int i = B - 1; i >= 0; i--) x = max(x, x ^ b[i]);
        return x;
    }
};
// @end
