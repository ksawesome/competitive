/**
 *    author:  Kshitish Madbhavi
 *    problem: Problem 2. Cross Country Skiing
 *    source:  USACO - USACO 2014 January Contest, Silver
 *    url:     https://usaco.org/index.php?page=viewproblem2&cpid=380
 *    created: 2026-10-05 07:55:02
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
    freopen("ccski.in", "r", stdin);
    freopen("ccski.out", "w", stdout);
#endif
}


void ff(int x, int y, int D, vector<vector<int>>& grid, vector<vector<int>>& elevations, vector<vector<bool>>& visited) {
    int m = sz(grid);
    int n = sz(grid[0]);
    visited[x][y] = true;

    if ((x + 1 < m) && (!visited[x + 1][y]) && (abs(elevations[x + 1][y] - elevations[x][y]) <= D)) {
        ff(x + 1, y, D, grid, elevations, visited);
    }
    if ((x - 1 >= 0) && (!visited[x - 1][y]) && (abs(elevations[x - 1][y] - elevations[x][y]) <= D)) {
        ff(x - 1, y, D, grid, elevations, visited);
    }
    if ((y + 1 < n) && (!visited[x][y + 1]) && (abs(elevations[x][y + 1] - elevations[x][y]) <= D)) {
        ff(x, y + 1, D, grid, elevations, visited);
    }
    if ((y - 1 >= 0) && (!visited[x][y - 1]) && (abs(elevations[x][y - 1] - elevations[x][y]) <= D)) {
        ff(x, y - 1, D, grid, elevations, visited);
    }
}

bool chk(int k, vector<pair<int, int>>& waypoints, vector<vector<int>>& grid, vector<vector<int>>& elevations) {
    auto start = waypoints[0];
    auto x = start.first;
    auto y = start.second;
    int m = sz(grid);
    int n = sz(grid[0]);

    vector<vector<bool>> visited(m, vector<bool>(n, false));

    ff(x, y, k, grid, elevations, visited);

    bool ans = true;

    for (const auto& [i, j] : waypoints) {
        if (!visited[i][j]) {
            ans = false;
            break;
        }
    }

    return ans;

}


void solve() {
    int m, n;
    cin >> m >> n;

    vector<vector<int>> elevations(m, vector<int>(n));
    rep(i, m) {
        rep(j, n) {
            cin >> elevations[i][j];
        }
    }

    vector<vector<int>> grid(m, vector<int>(n));
    rep(i, m) {
        rep(j, n) {
            cin >> grid[i][j];
        }
    }

    
    vector<pair<int, int>> waypoints;

    rep(i, m) {
        rep(j, n) {
            if (grid[i][j] == 1) waypoints.emplace_back(i, j);
        }
    }

    
    ll hi = 0;

rep(i, m) {
    rep(j, n) {
        if (i + 1 < m)
            hi = max(hi, abs(1LL * elevations[i + 1][j] - elevations[i][j]));

        if (j + 1 < n)
            hi = max(hi, abs(1LL * elevations[i][j + 1] - elevations[i][j]));
    }
}

ll lo = 0;
while (lo < hi) {
    ll mid = lo + (hi - lo) / 2;
    if (chk(mid, waypoints, grid, elevations)) hi = mid;
    else lo = mid + 1;
}

cout << lo;


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
