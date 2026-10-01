// @name   Euler's totient
// @prefix cpphi
// @key    y shift+e
// @desc   [global] phi(n) by trial division; phi_sieve(N) = phi of every x <= N. phi(n) counts 1 <= k <= n with gcd(k, n) = 1.
// @usage  ll p = phi(n);
// @usage  auto ph = phi_sieve(1000000);  // a^phi(m) = 1 (mod m) when gcd(a, m) = 1
// @complexity O(sqrt n) single, O(N log log N) sieve
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
ll phi(ll n) {
    ll r = n;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p) continue;
        while (n % p == 0) n /= p;
        r -= r / p;
    }
    if (n > 1) r -= r / n;
    return r;
}
vector<int> phi_sieve(int N) {
    vector<int> ph(N + 1);
    iota(ph.begin(), ph.end(), 0);
    for (int i = 2; i <= N; i++)
        if (ph[i] == i)
            for (int j = i; j <= N; j += i) ph[j] -= ph[j] / i;
    return ph;
}
// @end
