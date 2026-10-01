// @name   Miller-Rabin + Pollard rho (numbers up to 1e18)
// @prefix cppollard
// @key    y x
// @desc   [global] BigFactor::is_prime(n) (deterministic below 2^64), BigFactor::factor(n) -> prime factors with repeats, unsorted.
// @usage  bool p = BigFactor::is_prime(n);
// @usage  auto f = BigFactor::factor(n); sort(f.begin(), f.end());
// @complexity ~O(n^(1/4) log n) per factorization
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct BigFactor {
    using ull = unsigned long long;
    static ull mulmod(ull a, ull b, ull M) {  // valid for M < 7.2e18, via long double (no __int128)
        ll ret = a * b - M * ull(1.L / M * a * b);
        return ret + M * (ret < 0) - M * (ret >= (ll)M);
    }
    static ull powmod(ull b, ull e, ull M) {
        ull r = 1;
        for (b %= M; e; e >>= 1, b = mulmod(b, b, M))
            if (e & 1) r = mulmod(r, b, M);
        return r;
    }
    static bool is_prime(ull n) {
        if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
        ull s = __builtin_ctzll(n - 1), d = n >> s;
        for (ull a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
            ull p = powmod(a % n, d, n), i = s;
            while (p != 1 && p != n - 1 && a % n && i--) p = mulmod(p, p, n);
            if (p != n - 1 && i != s) return false;
        }
        return true;
    }
    static ull rho(ull n) {  // some non-trivial factor of composite n
        ull x = 0, y = 0, t = 30, prd = 2, i = 1, q;
        auto f = [&](ull v) { return mulmod(v, v, n) + i; };
        while (t++ % 40 || gcd(prd, n) == 1) {
            if (x == y) x = ++i, y = f(x);
            if ((q = mulmod(prd, max(x, y) - min(x, y), n))) prd = q;
            x = f(x), y = f(f(y));
        }
        return gcd(prd, n);
    }
    static vector<ull> factor(ull n) {
        if (n == 1) return {};
        if (is_prime(n)) return {n};
        ull x = rho(n);
        auto l = factor(x), r = factor(n / x);
        l.insert(l.end(), r.begin(), r.end());
        return l;
    }
};
// @end
