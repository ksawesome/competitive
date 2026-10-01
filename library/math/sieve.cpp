// @name   Sieve (primes + smallest prime factor)
// @prefix cpsieve
// @key    t p
// @desc   [global] init_sieve(N); primes, spf[x], is_prime(x), factor_small(x) -> {p, e} in O(log x) for x <= N.
// @usage  init_sieve(10000000);
// @usage  is_prime(x); primes; factor_small(x);  // x <= N
// @complexity O(N) init, O(log x) per factorization
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<int> spf, primes;  // spf[x] = smallest prime factor of x
void init_sieve(int N) {  // linear sieve, O(N)
    spf.assign(N + 1, 0);
    primes.clear();
    for (int i = 2; i <= N; i++) {
        if (spf[i] == 0) spf[i] = i, primes.push_back(i);
        for (int p : primes) {
            if (p > spf[i] || (ll)i * p > N) break;
            spf[i * p] = p;
        }
    }
}
bool is_prime(int x) { return x >= 2 && spf[x] == x; }
vector<pair<int, int>> factor_small(int x) {
    vector<pair<int, int>> f;
    while (x > 1) {
        int p = spf[x], e = 0;
        while (x % p == 0) x /= p, e++;
        f.push_back({p, e});
    }
    return f;
}
// @end
