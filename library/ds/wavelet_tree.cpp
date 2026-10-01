// @name   Wavelet tree (k-th smallest / count <= x in range)
// @prefix cpwavelet
// @key    y w
// @desc   [global] Wavelet wt(a): wt.kth(l, r, k) = k-th smallest (0-based) of a[l, r); wt.count_leq(l, r, x). Any ll values (compressed inside); a bit-packed wavelet matrix, so all levels stay in cache.
// @usage  Wavelet wt(a);
// @usage  ll med = wt.kth(l, r, (r - l) / 2); int c = wt.count_leq(l, r, x);
// @complexity O(n log n) build, O(log n) per query
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
struct Wavelet {
    int n = 0, lg = 1;
    vector<ll> vals;                // the sorted distinct values; the matrix stores their ranks
    vector<vector<uint64_t>> bit;   // bit[h]: bit h of every element, in the order of level h
    vector<vector<int32_t>> pre;    // pre[h][w]: the number of 1s in the words before w
    vector<int> zeros;              // zeros[h]: elements with bit h = 0; they come first at the next level
    template <class U>
    Wavelet(const vector<U>& a) : n(a.size()) {
        vals.assign(a.begin(), a.end());
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        vector<int> cur(n), nxt(n);
        for (int i = 0; i < n; i++) cur[i] = lower_bound(vals.begin(), vals.end(), (ll)a[i]) - vals.begin();
        while ((1 << lg) < (int)vals.size()) lg++;
        int words = n / 64 + 1;
        bit.assign(lg, vector<uint64_t>(words, 0));
        pre.assign(lg, vector<int32_t>(words + 1, 0));
        zeros.assign(lg, 0);
        for (int h = lg - 1; h >= 0; h--) {  // the highest bit first
            for (int i = 0; i < n; i++)
                if (cur[i] >> h & 1) bit[h][i >> 6] |= 1ULL << (i & 63);
            for (int w = 0; w < words; w++) pre[h][w + 1] = pre[h][w] + __builtin_popcountll(bit[h][w]);
            int z = 0;  // stable partition for the next level: zeros, then ones
            for (int i = 0; i < n; i++)
                if (!(cur[i] >> h & 1)) nxt[z++] = cur[i];
            zeros[h] = z;
            for (int i = 0; i < n; i++)
                if (cur[i] >> h & 1) nxt[z++] = cur[i];
            swap(cur, nxt);
        }
    }
    ll kth(int l, int r, int k) const {  // 0 <= k < r - l
        int res = 0;
        for (int h = lg - 1; h >= 0; h--) {
            int ol = ones(h, l), orr = ones(h, r), z = (r - l) - (orr - ol);
            if (k < z) l -= ol, r -= orr;  // the answer has bit h = 0
            else k -= z, res |= 1 << h, l = zeros[h] + ol, r = zeros[h] + orr;
        }
        return vals[res];
    }
    int count_leq(int l, int r, ll v) const {  // elements <= v in [l, r)
        int x = upper_bound(vals.begin(), vals.end(), v) - vals.begin();  // count the ranks < x
        if (x >= (1 << lg)) return r - l;
        int res = 0;
        for (int h = lg - 1; h >= 0; h--) {
            int ol = ones(h, l), orr = ones(h, r);
            if (x >> h & 1) res += (r - l) - (orr - ol), l = zeros[h] + ol, r = zeros[h] + orr;
            else l -= ol, r -= orr;
        }
        return res;
    }

   private:
    int ones(int h, int i) const {  // 1s among the first i elements of level h
        return pre[h][i >> 6] + __builtin_popcountll(bit[h][i >> 6] & ((1ULL << (i & 63)) - 1));
    }
};
// @end
