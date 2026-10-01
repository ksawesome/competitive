// @name   LCS + edit distance
// @prefix cplcs
// @key    t [
// @desc   [global] lcs(a, b) -> {length, one LCS}; edit_distance(a, b) = min insert / delete / replace operations.
// @usage  auto [len, s] = lcs(a, b);
// @usage  int d = edit_distance(a, b);
// @complexity O(nm)
#include <bits/stdc++.h>
using namespace std;

// @begin
pair<int, string> lcs(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = n - 1; i >= 0; i--)
        for (int j = m - 1; j >= 0; j--)
            dp[i][j] = a[i] == b[j] ? dp[i + 1][j + 1] + 1 : max(dp[i + 1][j], dp[i][j + 1]);
    string s;
    for (int i = 0, j = 0; i < n && j < m;) {
        if (a[i] == b[j]) s += a[i], i++, j++;
        else if (dp[i + 1][j] >= dp[i][j + 1]) i++;
        else j++;
    }
    return {dp[0][0], s};
}
int edit_distance(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            dp[i][j] = min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + (a[i - 1] != b[j - 1])});
    return dp[n][m];
}
// @end
