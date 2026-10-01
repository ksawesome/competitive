// @name   Z-function
// @prefix cpz
// @key    t z
// @desc   [global] z_function(s): z[i] = longest common prefix of s and s[i..]; z[0] = 0.
// @usage  vector<int> z = z_function(pat + '#' + text);  // z[i] == |pat| -> match
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;

// @begin
vector<int> z_function(const string& s) {
    int n = s.size();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}
// @end
