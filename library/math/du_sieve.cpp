// @name   Du's sieve: prefix sums of phi and mu up to 1e10
// @prefix cpdusieve
// @key    u d
// @desc   [global] DuSieve ds(n_max); ds.mu_sum(n) = mu(1)+...+mu(n) (exact), ds.phi_sum(n) = phi(1)+...+phi(n) mod MOD.
// @usage  DuSieve ds(10'000'000'000LL);
// @usage  ll M = ds.mu_sum(n), Phi = ds.phi_sum(n);
// @complexity O(n^(2/3)) for the first query, memoised after
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
struct DuSieve {
    ll lim;                     // values <= lim come from a linear sieve
    vector<ll> smu, sphi;       // prefix sums up to lim (phi mod MOD)
    unordered_map<ll, ll> mmu, mphi;
    DuSieve(ll n_max) {
        lim = max<ll>(10, (ll)cbrtl((long double)n_max * n_max) + 1);  // about n^(2/3)
        lim = min<ll>(lim, max<ll>(n_max, 10));
        vector<int> mu(lim + 1), primes;
        vector<ll> phi(lim + 1);
        vector<bool> comp(lim + 1);
        mu[1] = 1, phi[1] = 1;
        for (ll i = 2; i <= lim; i++) {
            if (!comp[i]) primes.push_back(i), mu[i] = -1, phi[i] = i - 1;
            for (int p : primes) {
                if ((ll)p * i > lim) break;
                comp[p * i] = true;
                if (i % p == 0) { mu[p * i] = 0, phi[p * i] = phi[i] * p; break; }
                mu[p * i] = -mu[i], phi[p * i] = phi[i] * (p - 1);
            }
        }
        smu.assign(lim + 1, 0), sphi.assign(lim + 1, 0);
        for (ll i = 1; i <= lim; i++) smu[i] = smu[i - 1] + mu[i], sphi[i] = (sphi[i - 1] + phi[i]) % MOD;
    }
    // sum over d | k of mu(d) = [k = 1]  =>  sum_{d=1}^{n} M(n / d) = 1
    ll mu_sum(ll n) {
        if (n <= lim) return smu[n];
        auto it = mmu.find(n);
        if (it != mmu.end()) return it->second;
        ll r = 1;
        for (ll l = 2, h; l <= n; l = h + 1) {
            h = n / (n / l);
            r -= (h - l + 1) * mu_sum(n / l);
        }
        return mmu[n] = r;
    }
    // sum over d | k of phi(d) = k  =>  sum_{d=1}^{n} Phi(n / d) = n (n + 1) / 2
    ll phi_sum(ll n) {
        if (n <= lim) return sphi[n];
        auto it = mphi.find(n);
        if (it != mphi.end()) return it->second;
        ll a = n % 2 == 0 ? (n / 2) % MOD : n % MOD, b = n % 2 == 0 ? (n + 1) % MOD : ((n + 1) / 2) % MOD;
        ll r = a * b % MOD;
        for (ll l = 2, h; l <= n; l = h + 1) {
            h = n / (n / l);
            r = (r - (h - l + 1) % MOD * phi_sum(n / l)) % MOD;
        }
        return mphi[n] = (r + MOD) % MOD;
    }
};
// @end
