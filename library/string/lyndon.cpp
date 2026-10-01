// @name   Lyndon factorization (Duval) and minimal rotation
// @prefix cplyndon
// @key    u y
// @desc   [global] duval(s): start indices of the Lyndon factorization s = w1 w2 ... wk with w1 >= w2 >= ... (each wi strictly smaller than all its rotations); min_rotation(s): start index of the lexicographically smallest rotation.
// @usage  vector<int> starts = duval(s);
// @usage  int k = min_rotation(s); string canon = s.substr(k) + s.substr(0, k);
// @complexity O(n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
vector<int> duval(const string& s) {
    int n = s.size(), i = 0;
    vector<int> starts;
    while (i < n) {  // s[i..j) is a pre-Lyndon prefix: repetitions of a Lyndon word of length j - k, plus a prefix of it
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) starts.push_back(i), i += j - k;
    }
    return starts;
}
int min_rotation(const string& s) {
    string d = s + s;
    int n = d.size(), i = 0, ans = 0;
    while (i < n / 2) {  // Duval on s + s: the last factor starting in the first half begins the minimal rotation
        ans = i;
        int j = i + 1, k = i;
        while (j < n && d[k] <= d[j]) {
            if (d[k] < d[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) i += j - k;
    }
    return ans;
}
// @end
