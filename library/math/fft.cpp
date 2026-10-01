// @name   FFT (exact integer convolution)
// @prefix cpfft
// @key    y f
// @desc   [global] multiply(a, b): convolution c[k] = sum a[i] b[k - i] with complex FFT; exact while results stay below ~1e15.
// @usage  vector<ll> c = multiply(a, b);  // polynomial product, counting pairs with a given sum, ...
// @complexity O((n + m) log(n + m))
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
using cd = complex<double>;
void fft(vector<cd>& a) {
    int n = a.size(), L = __builtin_ctz(n);
    static vector<complex<long double>> R(2, 1);
    static vector<cd> rt(2, 1);  // rt[k + j] = e^(i pi j / k), precomputed in long double for accuracy
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n), rt.resize(n);
        auto x = polar(1.0L, acosl(-1) / k);
        for (int i = k; i < 2 * k; i++) rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
    }
    vector<int> rev(n);
    for (int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    for (int i = 0; i < n; i++)
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += 2 * k)
            for (int j = 0; j < k; j++) {
                cd z = rt[j + k] * a[i + j + k];
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
}
template <class T>
vector<ll> multiply(const vector<T>& a, const vector<T>& b) {
    if (a.empty() || b.empty()) return {};
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    vector<cd> in(n), out(n);
    for (size_t i = 0; i < a.size(); i++) in[i].real((double)a[i]);
    for (size_t i = 0; i < b.size(); i++) in[i].imag((double)b[i]);
    fft(in);
    for (cd& x : in) x *= x;  // (a + ib)^2 = a^2 - b^2 + 2iab: the imaginary part holds 2ab
    for (int i = 0; i < n; i++) out[i] = in[-i & (n - 1)] - conj(in[i]);
    fft(out);
    vector<ll> res(need);
    for (int i = 0; i < need; i++) res[i] = llround(imag(out[i]) / (4.0 * n));
    return res;
}
// @end
