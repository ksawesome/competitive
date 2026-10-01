// @name   Pascal triangle (nCr for any modulus)
// @prefix cppascal
// @key    y shift+n
// @desc   [global] pascal(N, m)[n][r] = C(n, r) mod m for n <= N; works when m is not prime or is 1e18-ish (then keep N small).
// @usage  auto C = pascal(60, 1e18);  // exact binomials up to C(60, 30) ~ 1.2e17
// @complexity O(N^2)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<vector<ll>> pascal(int N, ll m) {
    vector<vector<ll>> C(N + 1, vector<ll>(N + 1, 0));
    for (int n = 0; n <= N; n++) {
        C[n][0] = 1 % m;
        for (int r = 1; r <= n; r++) C[n][r] = (C[n - 1][r - 1] + C[n - 1][r]) % m;
    }
    return C;
}
// @end
