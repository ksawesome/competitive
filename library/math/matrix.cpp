// @name   Matrix exponentiation
// @prefix cpmatrix
// @key    t shift+x
// @desc   [global] Mat R = mat_pow(A, e), mat_mul(A, B), entries mod MOD. Linear recurrences in O(k^3 log e).
// @usage  Mat M = {{1, 1}, {1, 0}};
// @usage  ll fib_n = mat_pow(M, n)[0][1];
// @complexity O(k^3 log e)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MOD = 1'000'000'007;

// @begin
using Mat = vector<vector<ll>>;  // entries must already be in [0, MOD)
Mat mat_mul(const Mat& A, const Mat& B) {
    int n = A.size(), k = B.size(), m = B[0].size();
    Mat C(n, vector<ll>(m));
    for (int i = 0; i < n; i++)
        for (int l = 0; l < k; l++) {
            if (!A[i][l]) continue;
            for (int j = 0; j < m; j++) C[i][j] = (C[i][j] + A[i][l] * B[l][j]) % MOD;
        }
    return C;
}
Mat mat_pow(Mat A, ll e) {
    int n = A.size();
    Mat R(n, vector<ll>(n));
    for (int i = 0; i < n; i++) R[i][i] = 1;
    for (; e > 0; e >>= 1, A = mat_mul(A, A))
        if (e & 1) R = mat_mul(R, A);
    return R;
}
// @end
