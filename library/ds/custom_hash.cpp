// @name   Anti-hack hash for unordered_map
// @prefix cphash
// @key    t shift+h
// @desc   [global] unordered_map<ll, int, custom_hash> mp; also works for pair keys. Stops anti-hash TLE hacks.
// @usage  unordered_map<ll, int, custom_hash> cnt;
// @usage  unordered_map<pair<int, int>, int, custom_hash> seen;
// @complexity O(1) expected per operation
#include <bits/stdc++.h>
using namespace std;

// @begin
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t seed = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + seed);
    }
    template <class A, class B>
    size_t operator()(const pair<A, B>& p) const {
        return (*this)(splitmix64(p.first) + p.second);
    }
};
// @end
