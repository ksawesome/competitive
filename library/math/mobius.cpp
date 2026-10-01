// @name   Mobius function sieve
// @prefix cpmobius
// @key    y shift+m
// @desc   [global] mobius_sieve(N) = mu[0..N] by a linear sieve. Key identity: sum over d | g of mu(d) = [g == 1] (count coprime pairs, etc.).
// @usage  auto mu = mobius_sieve(N);
// @usage  // pairs (a, b) <= N with gcd 1: sum over d of mu[d] * (N / d) * (N / d)
// @complexity O(N)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<int> mobius_sieve(int N) {
    vector<int> mu(N + 1), lp(N + 1), ps;
    if (N >= 1) mu[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (!lp[i]) lp[i] = i, ps.push_back(i), mu[i] = -1;
        for (int p : ps) {
            if (p > lp[i] || (ll)i * p > N) break;
            lp[i * p] = p;
            mu[i * p] = p == lp[i] ? 0 : -mu[i];
        }
    }
    return mu;
}
// @end
