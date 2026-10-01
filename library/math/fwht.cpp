// @name   Walsh-Hadamard transform: XOR / AND / OR convolution
// @prefix cpfwht
// @key    u w
// @desc   [global] xor_convolution(a, b), and_convolution(a, b), or_convolution(a, b): c[k] = sum of a[i]*b[j] over i^j = k (i&j, i|j) mod MOD. Sizes padded to a power of two.
// @usage  vector<ll> c = xor_convolution(a, b);
// @complexity O(n log n) for n = padded size
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
// kind 0 = XOR, 1 = AND, 2 = OR. Each butterfly mixes the pair (u, v) = (a[i], a[i + len]) that differ in one bit.
void fwht(vector<ll>& a, int kind, bool inverse) {
    int n = a.size();
    for (int len = 1; len < n; len <<= 1)
        for (int i = 0; i < n; i += 2 * len)
            for (int j = i; j < i + len; j++) {
                ll u = a[j], v = a[j + len];
                if (kind == 0) a[j] = (u + v) % MOD, a[j + len] = (u - v + MOD) % MOD;
                else if (kind == 1) a[j] = inverse ? (u - v + MOD) % MOD : (u + v) % MOD;          // sum over supersets
                else a[j + len] = inverse ? (v - u + MOD) % MOD : (u + v) % MOD;                 // sum over subsets
            }
    if (kind == 0 && inverse) {
        ll inv = 1, b = n % MOD;
        for (ll e = MOD - 2; e; e >>= 1, b = b * b % MOD)
            if (e & 1) inv = inv * b % MOD;
        for (ll& x : a) x = x * inv % MOD;
    }
}
vector<ll> bit_convolution(vector<ll> a, vector<ll> b, int kind) {
    size_t n = 1;
    while (n < max(a.size(), b.size())) n <<= 1;
    a.resize(n), b.resize(n);
    for (ll& x : a) x = (x % MOD + MOD) % MOD;
    for (ll& x : b) x = (x % MOD + MOD) % MOD;
    fwht(a, kind, false), fwht(b, kind, false);
    for (size_t i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;
    fwht(a, kind, true);
    return a;
}
vector<ll> xor_convolution(const vector<ll>& a, const vector<ll>& b) { return bit_convolution(a, b, 0); }
vector<ll> and_convolution(const vector<ll>& a, const vector<ll>& b) { return bit_convolution(a, b, 1); }
vector<ll> or_convolution(const vector<ll>& a, const vector<ll>& b) { return bit_convolution(a, b, 2); }
// @end
