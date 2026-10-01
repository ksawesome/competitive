// @name   KMP (prefix function + pattern search)
// @prefix cpkmp
// @key    t shift+k
// @desc   [global] prefix_function(s) -> pi; find_occurrences(text, pat) -> start index of every match.
// @usage  vector<int> starts = find_occurrences(text, pat);
// @usage  vector<int> pi = prefix_function(s);
// @complexity O(n + m)
#include <bits/stdc++.h>
using namespace std;

// @begin
vector<int> prefix_function(const string& s) {
    int n = s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}
vector<int> find_occurrences(const string& text, const string& pat) {
    if (pat.empty()) return {};
    int m = pat.size();
    vector<int> pi = prefix_function(pat + '\0' + text), res;
    for (int i = m + 1; i < (int)pi.size(); i++)
        if (pi[i] == m) res.push_back(i - 2 * m);
    return res;
}
// @end
