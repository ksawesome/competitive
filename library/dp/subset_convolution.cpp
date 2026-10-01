// @name   Subset convolution
// @prefix cpsubsetconv
// @key    u shift+s
// @desc   [global] subset_convolution(a, b): c[S] = sum over T subset of S of a[T] * b[S \ T] (disjoint unions only), mod MOD. Sizes 2^n; n = 20 takes about 1 s and 176 MB.
// @usage  vector<ll> c = subset_convolution(a, b);   // a.size() == b.size() == 1 << n
// @complexity O(n^2 2^n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
vector<ll> subset_convolution(const vector<ll>& a, const vector<ll>& b) {
    const uint32_t M = MOD;
    int N = a.size(), n = __builtin_ctz(N), R = n + 1;
    // ranked zeta transforms, row S holding ranks 0..n:
    // fa[S * R + k] = sum of a[T] over the subsets T of S with |T| = k
    vector<uint32_t> fa(N * R, 0), fb(N * R, 0);
    for (int S = 0; S < N; S++) {
        int k = __builtin_popcount(S);
        fa[S * R + k] = (a[S] % MOD + MOD) % MOD, fb[S * R + k] = (b[S] % MOD + MOD) % MOD;
    }
    auto zeta = [&](vector<uint32_t>& f) {  // ranks above |S \ bit| are still 0 there: skip them
        for (int bit = 1; bit < N; bit <<= 1)
            for (int S = bit; S < N; S = (S + 1) | bit) {
                uint32_t *x = &f[S * R], *y = &f[(S ^ bit) * R];
                for (int k = 0, lim = __builtin_popcount(S); k < lim; k++)
                    x[k] = x[k] + y[k] >= M ? x[k] + y[k] - M : x[k] + y[k];
            }
    };
    zeta(fa), zeta(fb);
    for (int S = 0; S < N; S++) {  // multiply the rank polynomials of S in place, high ranks first
        uint32_t *x = &fa[S * R], *y = &fb[S * R];
        for (int k = R - 1; k >= 0; k--) {
            unsigned long long acc = 0;
            for (int i = 0; i <= k; i++) {
                acc += (unsigned long long)x[i] * y[k - i];
                if ((i & 15) == 15) acc %= M;  // 16 products of < M^2 fit in 64 bits
            }
            x[k] = acc % M;
        }
    }
    for (int bit = 1; bit < N; bit <<= 1)  // the inverse (Moebius) transform, all ranks
        for (int S = bit; S < N; S = (S + 1) | bit) {
            uint32_t *x = &fa[S * R], *y = &fa[(S ^ bit) * R];
            for (int k = 0; k < R; k++) x[k] = x[k] >= y[k] ? x[k] - y[k] : x[k] + M - y[k];
        }
    vector<ll> c(N);
    for (int S = 0; S < N; S++) c[S] = fa[S * R + __builtin_popcount(S)];  // |T| + |S \ T| = |S| forces disjointness
    return c;
}
// @end
