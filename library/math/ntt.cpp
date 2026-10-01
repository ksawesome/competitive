// @name   NTT (convolution mod 998244353)
// @prefix cpntt
// @key    y shift+t
// @desc   [global] ntt_multiply(a, b): convolution with every coefficient mod 998244353, exact for any size up to 2^23.
// @usage  vector<ll> c = ntt_multiply(a, b);
// @complexity O((n + m) log(n + m))
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
const ll NTT_MOD = 998244353, NTT_ROOT = 3;
ll ntt_pow(ll b, ll e) {
    ll r = 1;
    for (b %= NTT_MOD; e; e >>= 1, b = b * b % NTT_MOD)
        if (e & 1) r = r * b % NTT_MOD;
    return r;
}
void ntt(vector<ll>& a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll w = ntt_pow(NTT_ROOT, (NTT_MOD - 1) / len);
        if (invert) w = ntt_pow(w, NTT_MOD - 2);
        for (int i = 0; i < n; i += len) {
            ll wn = 1;
            for (int j = 0; j < len / 2; j++) {
                ll u = a[i + j], v = a[i + j + len / 2] * wn % NTT_MOD;
                a[i + j] = u + v < NTT_MOD ? u + v : u + v - NTT_MOD;
                a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + NTT_MOD;
                wn = wn * w % NTT_MOD;
            }
        }
    }
    if (invert) {
        ll inv_n = ntt_pow(n, NTT_MOD - 2);
        for (ll& x : a) x = x * inv_n % NTT_MOD;
    }
}
template <class T>
vector<ll> ntt_multiply(const vector<T>& a, const vector<T>& b) {
    if (a.empty() || b.empty()) return {};
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    vector<ll> fa(n), fb(n);
    for (size_t i = 0; i < a.size(); i++) fa[i] = ((ll)a[i] % NTT_MOD + NTT_MOD) % NTT_MOD;
    for (size_t i = 0; i < b.size(); i++) fb[i] = ((ll)b[i] % NTT_MOD + NTT_MOD) % NTT_MOD;
    ntt(fa, false), ntt(fb, false);
    for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i] % NTT_MOD;
    ntt(fa, true);
    fa.resize(need);
    return fa;
}
// @end
