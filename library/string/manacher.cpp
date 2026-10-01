// @name   Manacher (all palindromes)
// @prefix cpmanacher
// @key    y 6
// @desc   [global] manacher(s) -> {d1, d2}: d1[i] = odd palindromes centred at i, d2[i] = even ones centred before i; longest_palindrome(s) -> {start, len}.
// @usage  auto [start, len] = longest_palindrome(s);
// @usage  // number of palindromic substrings = sum(d1) + sum(d2)
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

// @begin
pair<vector<int>, vector<int>> manacher(const string& s) {
    int n = s.size();
    vector<int> d1(n), d2(n);
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
        d1[i] = k--;
        if (i + k > r) l = i - k, r = i + k;
    }
    for (int i = 0, l = 0, r = -1; i < n; i++) {
        int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        d2[i] = k--;
        if (i + k > r) l = i - k - 1, r = i + k;
    }
    return {d1, d2};
}
pair<int, int> longest_palindrome(const string& s) {
    auto [d1, d2] = manacher(s);
    int start = 0, len = s.empty() ? 0 : 1;
    for (int i = 0; i < (int)s.size(); i++) {
        if (2 * d1[i] - 1 > len) len = 2 * d1[i] - 1, start = i - d1[i] + 1;
        if (2 * d2[i] > len) len = 2 * d2[i], start = i - d2[i];
    }
    return {start, len};
}
// @end
