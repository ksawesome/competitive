/**
 *    author:  Kshitish Madbhavi
 *    problem: F. Minimize the Diameter
 *    source:  Codeforces - SashaT9 Contest 1
 *    url:     https://codeforces.com/gym/104536/problem/F
 *    created: 2026-10-15 14:28:00
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

pair<int, vector<int>> tree_diameter(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> dist(n), par(n);
    auto farthest = [&](int s) {
        fill(dist.begin(), dist.end(), -1);
        dist[s] = 0;
        par[s] = -1;
        queue<int> q;
        q.push(s);
        int last = s;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            last = u;  // BFS pops in distance order, so the last one is farthest
            for (int v : adj[u])
                if (dist[v] == -1) dist[v] = dist[u] + 1, par[v] = u, q.push(v);
        }
        return last;
    };
    int a = farthest(0), b = farthest(a);
    vector<int> path;
    for (int v = b; v != -1; v = par[v]) path.push_back(v);
    return {dist[b], path};
}

void solve() {
    int n;
    cin >> n;

    vector<vector<int>> adj_1(n);
    rep(i, n - 1) {
        int a, b;
        cin >> a >> b;
        a--;b--;
        adj_1[a].push_back(b);
        adj_1[b].push_back(a);
    }

    int m;
    cin >> m;

    vector<vector<int>> adj_2(m);              // size m, 0-indexed
rep(i, m - 1) {
    int a, b;
    cin >> a >> b;
    a--; b--;
    adj_2[a].push_back(b);
    adj_2[b].push_back(a);
}

auto dia_1 = tree_diameter(adj_1);
auto dia_2 = tree_diameter(adj_2);

vector<int> cands_1 = {dia_1.second[dia_1.first / 2]};
if (dia_1.first % 2 == 1) cands_1.push_back(dia_1.second[dia_1.first / 2 + 1]);

vector<int> cands_2 = {dia_2.second[dia_2.first / 2]};
if (dia_2.first % 2 == 1) cands_2.push_back(dia_2.second[dia_2.first / 2 + 1]);

int ans = INF;
for (int c1 : cands_1) {
    for (int c2 : cands_2) {
        vector<vector<int>> adj(n + m);
        rep(i, n) adj[i] = adj_1[i];
        rep(i, m) for (int v : adj_2[i]) adj[n + i].push_back(v + n);   // offset here
        adj[c1].push_back(c2 + n);
        adj[c2 + n].push_back(c1);
        chmin(ans, tree_diameter(adj).first);
    }
}
cout << ans << '\n';
    
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(10);
    file_io();

    int t = 1;
    // cin >> t;
    while (t--) solve();
}
