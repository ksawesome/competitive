// @name   Polynomial toolkit mod 998244353 (inverse, divmod, log, exp, sqrt, pow, multipoint)
// @prefix cppoly
// @key    u shift+p
// @desc   [global] Poly = vector<ll>. poly_mul, poly_inv(a, n), poly_divmod(a, b), poly_log / poly_exp / poly_sqrt (a, n), poly_pow(a, k, n), poly_eval_many(a, xs), poly_interpolate(xs, ys). Self-contained NTT.
// @usage  Poly b = poly_inv(a, n);   // a * b = 1 mod x^n, needs a[0] != 0
// @usage  auto [q, r] = poly_divmod(a, b);  Poly e = poly_exp(a, n);  vector<ll> v = poly_eval_many(a, xs);
// @complexity O(n log n) for mul / inv / divmod / log / exp / sqrt / pow; O(n log^2 n) for multipoint and interpolation
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
const ll PMOD = 998244353;
using Poly = vector<ll>;
ll poly_pw(ll b, ll e) {
    ll r = 1;
    for (b %= PMOD; e; e >>= 1, b = b * b % PMOD)
        if (e & 1) r = r * b % PMOD;
    return r;
}
void poly_ntt(Poly& a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll w = poly_pw(3, (PMOD - 1) / len);
        if (invert) w = poly_pw(w, PMOD - 2);
        for (int i = 0; i < n; i += len) {
            ll wn = 1;
            for (int j = 0; j < len / 2; j++, wn = wn * w % PMOD) {
                ll u = a[i + j], v = a[i + j + len / 2] * wn % PMOD;
                a[i + j] = u + v < PMOD ? u + v : u + v - PMOD;
                a[i + j + len / 2] = u >= v ? u - v : u - v + PMOD;
            }
        }
    }
    if (invert) {
        ll inv_n = poly_pw(n, PMOD - 2);
        for (ll& x : a) x = x * inv_n % PMOD;
    }
}
Poly poly_mul(Poly a, Poly b) {
    if (a.empty() || b.empty()) return {};
    size_t need = a.size() + b.size() - 1, n = 1;
    if (min(a.size(), b.size()) <= 40) {  // small: schoolbook is faster
        Poly c(need, 0);
        for (size_t i = 0; i < a.size(); i++)
            for (size_t j = 0; j < b.size(); j++) c[i + j] = (c[i + j] + a[i] * b[j]) % PMOD;
        return c;
    }
    while (n < need) n <<= 1;
    a.resize(n), b.resize(n);
    poly_ntt(a, false), poly_ntt(b, false);
    for (size_t i = 0; i < n; i++) a[i] = a[i] * b[i] % PMOD;
    poly_ntt(a, true);
    a.resize(need);
    return a;
}
Poly poly_cut(Poly a, size_t n) { a.resize(n, 0); return a; }
Poly poly_inv(const Poly& a, size_t n) {  // b with a * b = 1 (mod x^n); Newton: b <- b (2 - a b)
    Poly b{poly_pw(a[0], PMOD - 2)};
    for (size_t k = 1; k < n; k <<= 1) {
        Poly ab = poly_cut(poly_mul(poly_cut(a, 2 * k), b), 2 * k);
        for (ll& x : ab) x = (PMOD - x) % PMOD;
        ab[0] = (ab[0] + 2) % PMOD;
        b = poly_cut(poly_mul(b, ab), 2 * k);
    }
    return poly_cut(b, n);
}
pair<Poly, Poly> poly_divmod(Poly a, Poly b) {  // a = b q + r, deg r < deg b; b must not be zero
    while (!a.empty() && a.back() == 0) a.pop_back();
    while (!b.empty() && b.back() == 0) b.pop_back();
    if (a.size() < b.size()) return {{}, a};
    size_t k = a.size() - b.size() + 1;  // reversing turns the quotient into a power series division
    Poly ra(a.rbegin(), a.rend()), rb(b.rbegin(), b.rend());
    Poly q = poly_cut(poly_mul(poly_cut(ra, k), poly_inv(rb, k)), k);
    reverse(q.begin(), q.end());
    Poly r = poly_cut(poly_mul(b, q), b.size() - 1);
    for (size_t i = 0; i < r.size(); i++) r[i] = (a[i] - r[i] + PMOD) % PMOD;
    while (!r.empty() && r.back() == 0) r.pop_back();
    return {q, r};
}
Poly poly_log(const Poly& a, size_t n) {  // needs a[0] = 1: log a = integral of a' / a
    if (n == 0) return {};
    Poly da(n - 1, 0);
    for (size_t i = 1; i < min(a.size(), n); i++) da[i - 1] = a[i] * (ll)i % PMOD;
    Poly q = poly_cut(poly_mul(da, poly_inv(a, n)), n - 1), res(n, 0), inv(n + 1, 1);
    for (size_t i = 2; i <= n; i++) inv[i] = (PMOD - (PMOD / i) * inv[PMOD % i] % PMOD) % PMOD;
    for (size_t i = 1; i < n; i++) res[i] = q[i - 1] * inv[i] % PMOD;
    return res;
}
Poly poly_exp(const Poly& a, size_t n) {  // needs a[0] = 0; Newton: b <- b (1 - log b + a)
    Poly b{1};
    for (size_t k = 1; k < n; k <<= 1) {
        Poly l = poly_log(b, 2 * k);
        for (size_t i = 0; i < 2 * k; i++) l[i] = ((i < a.size() ? a[i] : 0) - l[i] + PMOD) % PMOD;
        l[0] = (l[0] + 1) % PMOD;
        b = poly_cut(poly_mul(b, l), 2 * k);
    }
    return poly_cut(b, n);
}
Poly poly_sqrt(const Poly& a, size_t n) {  // needs a[0] = 1; Newton: b <- (b + a / b) / 2
    Poly b{1};
    const ll inv2 = (PMOD + 1) / 2;
    for (size_t k = 1; k < n; k <<= 1) {
        Poly t = poly_cut(poly_mul(poly_cut(a, 2 * k), poly_inv(b, 2 * k)), 2 * k);
        b.resize(2 * k, 0);
        for (size_t i = 0; i < 2 * k; i++) b[i] = (b[i] + t[i]) * inv2 % PMOD;
    }
    return poly_cut(b, n);
}
Poly poly_pow(const Poly& a, ll k, size_t n) {  // a^k mod x^n for any k >= 0
    Poly zero(n, 0);
    if (n == 0) return zero;
    if (k == 0) { zero[0] = 1; return zero; }
    size_t t = 0;
    while (t < a.size() && a[t] == 0) t++;  // a = c x^t (1 + ...)
    if (t == a.size() || (t > 0 && k >= (ll)((n + t - 1) / t))) return zero;
    size_t shift = t * k;
    ll c = a[t], ic = poly_pw(c, PMOD - 2);
    Poly b(a.begin() + t, a.end());
    for (ll& x : b) x = x * ic % PMOD;
    Poly l = poly_log(poly_cut(b, n - shift), n - shift);
    for (ll& x : l) x = x * (k % PMOD) % PMOD;
    Poly e = poly_exp(l, n - shift);
    ll ck = poly_pw(c, k);
    for (size_t i = 0; i + shift < n; i++) zero[i + shift] = e[i] * ck % PMOD;
    return zero;
}
// subproduct tree over the points: node v holds prod (x - xs[i]) for i in its range
void poly_build_tree(vector<Poly>& tr, const vector<ll>& xs, int v, int l, int r) {
    if (r - l == 1) { tr[v] = {(PMOD - xs[l] % PMOD) % PMOD, 1}; return; }
    int m = (l + r) / 2;
    poly_build_tree(tr, xs, 2 * v, l, m), poly_build_tree(tr, xs, 2 * v + 1, m, r);
    tr[v] = poly_mul(tr[2 * v], tr[2 * v + 1]);
}
void poly_eval_rec(const vector<Poly>& tr, const Poly& f, const vector<ll>& xs, vector<ll>& out, int v, int l, int r) {
    if (r - l <= 32) {  // small ranges: Horner is faster than more divisions
        for (int i = l; i < r; i++) {
            ll y = 0, x = xs[i] % PMOD;
            for (size_t j = f.size(); j-- > 0;) y = (y * x + f[j]) % PMOD;
            out[i] = y;
        }
        return;
    }
    int m = (l + r) / 2;
    poly_eval_rec(tr, poly_divmod(f, tr[2 * v]).second, xs, out, 2 * v, l, m);
    poly_eval_rec(tr, poly_divmod(f, tr[2 * v + 1]).second, xs, out, 2 * v + 1, m, r);
}
vector<ll> poly_eval_many(const Poly& a, const vector<ll>& xs) {
    int m = xs.size();
    if (m == 0) return {};
    vector<Poly> tr(4 * m);
    poly_build_tree(tr, xs, 1, 0, m);
    vector<ll> out(m);
    poly_eval_rec(tr, poly_divmod(a, tr[1]).second, xs, out, 1, 0, m);
    return out;
}
Poly poly_interpolate(const vector<ll>& xs, const vector<ll>& ys) {  // distinct xs; the degree < n polynomial
    int m = xs.size();
    if (m == 0) return {};
    vector<Poly> tr(4 * m);
    poly_build_tree(tr, xs, 1, 0, m);
    Poly dm(tr[1].size() - 1);  // M'(x): the Lagrange denominators are M'(xs[i])
    for (size_t i = 1; i < tr[1].size(); i++) dm[i - 1] = tr[1][i] * (ll)i % PMOD;
    vector<ll> den(m);
    poly_eval_rec(tr, dm, xs, den, 1, 0, m);
    auto combine = [&](auto&& self, int v, int l, int r) -> Poly {
        if (r - l == 1) return {(ys[l] % PMOD + PMOD) % PMOD * poly_pw(den[l], PMOD - 2) % PMOD};
        int mid = (l + r) / 2;
        Poly left = poly_mul(self(self, 2 * v, l, mid), tr[2 * v + 1]);
        Poly right = poly_mul(self(self, 2 * v + 1, mid, r), tr[2 * v]);
        left.resize(max(left.size(), right.size()), 0);
        for (size_t i = 0; i < right.size(); i++) left[i] = (left[i] + right[i]) % PMOD;
        return left;
    };
    Poly res = combine(combine, 1, 0, m);
    res.resize(m, 0);
    return res;
}
// @end
