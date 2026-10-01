// @name   Radix sort and counting sort
// @prefix cpradix
// @key    u shift+r
// @desc   [global] radix_sort(a): sorts 64-bit integers (negatives too) in 4 linear passes of 16 bits, often 2-3x faster than std::sort for n >= 1e6; counting_order(keys, K): stable order of indices by small keys in [0, K).
// @usage  radix_sort(a);   // vector<ll>
// @usage  vector<int> ord = counting_order(keys, K);   // stable: equal keys keep their order
// @complexity O(n) per pass: O(4 (n + 65536)) for radix_sort, O(n + K) for counting_order
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
void radix_sort(vector<ll>& a) {
    size_t n = a.size();
    vector<uint64_t> k(n), tmp(n);
    for (size_t i = 0; i < n; i++) k[i] = (uint64_t)a[i] ^ (1ULL << 63);  // flip the sign bit: unsigned order = signed order
    vector<size_t> cnt(1 << 16);
    for (int shift = 0; shift < 64; shift += 16) {  // least significant digit first; each pass is stable
        fill(cnt.begin(), cnt.end(), 0);
        for (uint64_t x : k) cnt[(x >> shift) & 0xFFFF]++;
        size_t sum = 0;
        for (auto& c : cnt) { size_t t = c; c = sum; sum += t; }
        for (uint64_t x : k) tmp[cnt[(x >> shift) & 0xFFFF]++] = x;
        swap(k, tmp);
    }
    for (size_t i = 0; i < n; i++) a[i] = (ll)(k[i] ^ (1ULL << 63));
}
vector<int> counting_order(const vector<int>& keys, int K) {
    vector<int> cnt(K + 1, 0), ord(keys.size());
    for (int x : keys) cnt[x + 1]++;
    for (int i = 0; i < K; i++) cnt[i + 1] += cnt[i];
    for (size_t i = 0; i < keys.size(); i++) ord[cnt[keys[i]]++] = i;
    return ord;
}
// @end
