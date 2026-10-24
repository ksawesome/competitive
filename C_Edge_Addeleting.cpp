/**
 *    author:  Kshitish Madbhavi
 *    problem: C. Edge Addeleting
 *    source:  Codeforces - Codeforces Round 1129 (Div. 2)
 *    url:     https://codeforces.com/contest/2277/problem/C
 *    created: 2026-10-24 17:37:38
 */
#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL
#include "debug.h"
#else
#define dbg(...)
#endif

#define int long long

using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vvi = vector<vi>;
template <class T> using minpq = priority_queue<T, vector<T>, greater<T>>;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (int)(x).size()
#define PICK3(_1, _2, _3, NAME, ...) NAME
#define rep2(i, n) for (int i = 0; i < (n); i++)
#define rep3(i, a, b) for (int i = (a); i < (b); i++)
#define per2(i, n) for (int i = (n) - 1; i >= 0; i--)
#define per3(i, a, b) for (int i = (b) - 1; i >= (a); i--)
#define rep(...) PICK3(__VA_ARGS__, rep3, rep2, )(__VA_ARGS__)  // rep(i, n): 0..n-1   rep(i, a, b): a..b-1
#define per(...) PICK3(__VA_ARGS__, per3, per2, )(__VA_ARGS__)  // same ranges, backwards

constexpr ll INF = 4e18;
constexpr int MOD = 1'000'000'007;  // 998'244'353

template <class T, class U> bool chmin(T& a, const U& b) { if (b < a) { a = b; return true; } return false; }
template <class T, class U> bool chmax(T& a, const U& b) { if (a < b) { a = b; return true; } return false; }
ll cdiv(ll a, ll b) { return a / b + (a % b != 0 && (a < 0) == (b < 0)); }  // ceil(a / b), any signs
ll fdiv(ll a, ll b) { return a / b - (a % b != 0 && (a < 0) != (b < 0)); }  // floor(a / b), any signs

template <class T> istream& operator>>(istream& is, vector<T>& v) { for (auto& x : v) is >> x; return is; }
template <class A, class B> istream& operator>>(istream& is, pair<A, B>& p) { return is >> p.first >> p.second; }
template <class T> ostream& operator<<(ostream& os, const vector<T>& v) {
    for (size_t i = 0; i < v.size(); i++) os << (i ? " " : "") << v[i];
    return os;
}
template <class... T> void print(const T&... xs) {  // print(a, b, c) -> "a b c\n"
    [[maybe_unused]] int i = 0;
    ((cout << (i++ ? " " : "") << xs), ...);
    cout << '\n';
}
void yn(bool ok) { cout << (ok ? "YES" : "NO") << '\n'; }

// USACO-style file I/O switches on by itself: when CPH creates this file it fills the two raw strings
// below from the problem ("type":"file","fileName":"xxx.in" for USACO, "type":"stdin" otherwise).
// It only runs on the judge; locally (CPH, F5) input always comes from stdin.
void file_io() {
#ifndef LOCAL
    auto file = [](const string& spec) {
        size_t p = spec.find("\"fileName\":\"");
        return p == string::npos ? string() : spec.substr(p + 12, spec.find('"', p + 12) - p - 12);
    };
    string in = file(R"("type":"stdin")"), out = file(R"("type":"stdout")");
    if (!in.empty()) freopen(in.c_str(), "r", stdin);
    if (!out.empty()) freopen(out.c_str(), "w", stdout);
#endif
}

bool path(int curr, int end, vector<int>& pat, vector<vector<int>>& adj) {  
    if (curr == end) return true;
    int prev = sz(pat) > 1 ? pat[sz(pat) - 2] : -1;
    for (auto& next : adj[curr]) {
        if (next == prev) continue;
        pat.push_back(next);
        if (path(next, end, pat, adj)) return true;
        pat.pop_back();
    }
    return false;
}
 
pii max_distance(int curr, int prev, int next, vvi& adj, vi& pick) {  
    queue<pair<int, int>> q;
    map<int, int> dist;
    dist[-1] = -1;
    q.push({-1, curr});
 
    pair<int, int> ans = {-1, -1};
 
    while (!q.empty()) {
        int temp = q.front().second;
        int before = q.front().first;
        q.pop();
        dist[temp] = dist[before] + 1;
        if (pick[temp] && chmax(ans.second, dist[temp])) ans.first = temp;
        for(const auto& node : adj[temp]) {
            if(node == next || node == prev || node == before) continue;
            q.push({temp, node});
        }
    }
 
    return ans;
}
 
void solve() {
    int n, m;
    cin >> n >> m;
 
    vvi adj(n);
 
    rep(i, n-1) {
        int x, y;
        cin >> x >> y;
        x--;y--;
 
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
 
    vi a(m);
    cin >> a;
 
    vi pick(n, 0);
    for (auto x : a) pick[x - 1] = 1;
 
    vi pat = {0};
    path(0, n-1, pat, adj);
    int k = pat.size() - 1;
    int submitt = k;
 
    rep(i, 1, sz(pat)) {
        pii far = max_distance(pat[i], pat[i - 1], i + 1 < sz(pat) ? pat[i + 1] : -1, adj, pick);
        if (far.first != -1) chmax(submitt, far.second + 1 + (k - i));
    }
 
    cout << submitt << '\n';
 
}


signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(10);
    file_io();

    int t = 1;
    cin >> t;
    while (t--) solve();
}
