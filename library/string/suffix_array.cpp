// @name   Suffix array + LCP
// @prefix cpsa
// @key    y 4
// @desc   [global] suffix_array(s): sa[i] = start of the i-th smallest suffix; lcp_array(s, sa)[i] = LCP of suffixes sa[i] and sa[i + 1].
// @usage  auto sa = suffix_array(s); auto lcp = lcp_array(s, sa);
// @usage  // distinct substrings = n(n+1)/2 - sum(lcp)
// @complexity O(n log n)
#include <bits/stdc++.h>
using namespace std;

// @begin
vector<int> suffix_array(string s) {
    s += '\0';  // sentinel smaller than every character (s must not contain '\0')
    int n = s.size();
    vector<int> p(n), c(n), cnt(max<int>(256, n)), pn(n), cn(n);
    for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
    for (int i = 1; i < 256; i++) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--) p[--cnt[(unsigned char)s[i]]] = i;
    int classes = 1;
    for (int i = 1; i < n; i++) {
        if (s[p[i]] != s[p[i - 1]]) classes++;
        c[p[i]] = classes - 1;
    }
    for (int h = 0; (1 << h) < n; h++) {  // sort cyclic shifts by their first 2^(h+1) characters
        for (int i = 0; i < n; i++) pn[i] = (p[i] - (1 << h) + n) % n;
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int i = 0; i < n; i++) cnt[c[pn[i]]]++;
        for (int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0, classes = 1;
        for (int i = 1; i < n; i++) {
            pair<int, int> cur{c[p[i]], c[(p[i] + (1 << h)) % n]}, prev{c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
            if (cur != prev) classes++;
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
    }
    p.erase(p.begin());  // drop the sentinel suffix
    return p;
}
vector<int> lcp_array(const string& s, const vector<int>& sa) {  // Kasai
    int n = s.size();
    vector<int> rnk(n), lcp(n > 0 ? n - 1 : 0);
    for (int i = 0; i < n; i++) rnk[sa[i]] = i;
    for (int i = 0, k = 0; i < n; i++) {
        if (rnk[i] == n - 1) {
            k = 0;
            continue;
        }
        int j = sa[rnk[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rnk[i]] = k;
        if (k) k--;
    }
    return lcp;
}
// @end
