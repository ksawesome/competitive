// @name   Mint (modular integer)
// @prefix cpmint
// @key    t shift+m
// @desc   [global] mint x = 5; x += 3; x *= x.inv(); x.pow(e); cout << x. Uses the template's MOD (must be prime for /).
// @usage  mint x = 5;
// @usage  x = x * 3 - 1; x /= 2; x.pow(10); cout << x;
// @complexity O(1) per op, O(log MOD) for / and inv
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
template <int M>
struct Mint {
    int v;
    Mint(ll x = 0) : v(static_cast<int>(x % M)) {
        if (v < 0) v += M;
    }
    Mint pow(ll e) const {
        Mint r = 1, b = *this;
        for (; e > 0; e >>= 1, b *= b)
            if (e & 1) r *= b;
        return r;
    }
    Mint inv() const { return pow(M - 2); }
    Mint& operator+=(Mint o) { v += o.v; if (v >= M) v -= M; return *this; }
    Mint& operator-=(Mint o) { v -= o.v; if (v < 0) v += M; return *this; }
    Mint& operator*=(Mint o) { v = static_cast<int>((ll)v * o.v % M); return *this; }
    Mint& operator/=(Mint o) { return *this *= o.inv(); }
    Mint operator-() const { return Mint() - *this; }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    friend Mint operator/(Mint a, Mint b) { return a /= b; }
    friend bool operator==(Mint a, Mint b) { return a.v == b.v; }
    friend bool operator!=(Mint a, Mint b) { return a.v != b.v; }
    friend ostream& operator<<(ostream& os, Mint a) { return os << a.v; }
    friend istream& operator>>(istream& is, Mint& a) { ll x; is >> x; a = Mint(x); return is; }
};
using mint = Mint<MOD>;
// @end
