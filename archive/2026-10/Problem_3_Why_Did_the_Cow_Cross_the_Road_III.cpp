/**
 *    author:  Kshitish Madbhavi
 *    problem: Problem 3. Why Did the Cow Cross the Road III
 *    source:  USACO - USACO 2017 February Contest, Silver
 *    url:     https://usaco.org/index.php?page=viewproblem2&cpid=716
 *    created: 2026-10-11 07:42:45
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
    string in = file(R"("fileName":"countcross.in","type":"file")"), out = file(R"("fileName":"countcross.out","type":"file")");
    if (!in.empty()) freopen(in.c_str(), "r", stdin);
    if (!out.empty()) freopen(out.c_str(), "w", stdout);
#endif
}



void solve() {
    int n, k, r;
    cin >> n >> k >> r;

    set<pair<pii, pii>> roads;
    rep(i, r) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        r1--; c1--; r2--; c2--;
        roads.insert({{r1, c1}, {r2, c2}});
        roads.insert({{r2, c2}, {r1, c1}});
    }

    vector<string> grid(n, string(n, '.'));
    vector<pii> cows(k);
    rep(i, k) {
        int x, y;
        cin >> x >> y;
        x--; y--;
        grid[x][y] = 'c';
        cows[i] = {x, y};
    }

    int m = n;
    const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    auto open = [&](int r, int c) { return grid[r][c] == '.' || grid[r][c] == 'c'; };  // which cells get filled
    vector<vector<int>> region(n, vector<int>(m, -1));  // region id per cell, -1 = not filled
    vector<int> region_size;
    for (int sr = 0; sr < n; sr++) {
        for (int sc = 0; sc < m; sc++) {
            if (!open(sr, sc) || region[sr][sc] != -1) continue;
            int id = region_size.size();
            region_size.push_back(0);
            queue<pii> q;
            q.push({sr, sc});
            region[sr][sc] = id;
            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                region_size[id]++;
                for (int d = 0; d < 4; d++) {
                    int nr = cr + dr[d], nc = cc + dc[d];
                    if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
                    if (!open(nr, nc) || region[nr][nc] != -1) continue;
                    if (roads.count({{cr, cc}, {nr, nc}})) continue;  // road blocks this step
                    region[nr][nc] = id;
                    q.push({nr, nc});
                }
            }
        }
    }

    // a pair is distant iff the two cows sit in different regions
    int ans = 0;
    rep(i, k) rep(j, i + 1, k) {
        if (region[cows[i].first][cows[i].second] != region[cows[j].first][cows[j].second]) ans++;
    }
    print(ans);
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
