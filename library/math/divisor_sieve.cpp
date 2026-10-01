// @name   Divisor sieve (count + sum of divisors)
// @prefix cpdivsieve
// @key    t shift+2
// @desc   [global] divisor_sieve(N) -> {cnt, sum}: number and sum of divisors of every x <= N by looping over multiples.
// @usage  auto [cnt, sum] = divisor_sieve(1000000);
// @complexity O(N log N)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
pair<vector<int>, vector<ll>> divisor_sieve(int N) {
    vector<int> cnt(N + 1);
    vector<ll> sum(N + 1);
    for (int d = 1; d <= N; d++)
        for (int x = d; x <= N; x += d) cnt[x]++, sum[x] += d;
    return {cnt, sum};
}
// @end
