/**
 *    author:  Kshitish Madbhavi
 *    problem: Problem 3. Where's Bessie?
 *    source:  USACO - USACO 2017 US Open Contest, Silver
 *    url:     https://usaco.org/index.php?page=viewproblem2&cpid=740
 *    created: 2026-10-09 04:12:06
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
    string in = file(R"("fileName":"where.in","type":"file")"), out = file(R"("fileName":"where.out","type":"file")");
    if (!in.empty()) freopen(in.c_str(), "r", stdin);
    if (!out.empty()) freopen(out.c_str(), "w", stdout);
#endif
}

// BFS flood fill from (u, v), staying inside the rectangle [r1..r2] x [c1..c2]. Returns the region's color.
char ff(int u, int v, int r1, int c1, int r2, int c2, const vector<string>& grid, vector<vector<bool>>& visited) {
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    queue<pair<int, int>> q;
    q.push({u, v});
    visited[u][v] = true;
    char typ = grid[u][v];

    while (!q.empty()) {
        auto temp = q.front();
        q.pop();
        int x = temp.first;
        int y = temp.second;

        rep(d, 4) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx < r1 || nx > r2 || ny < c1 || ny > c2) continue;
            if (visited[nx][ny] || grid[nx][ny] != typ) continue;
            visited[nx][ny] = true;
            q.push({nx, ny});
        }
    }

    return typ;
}

// number of connected regions of each color present inside the rectangle [r1..r2] x [c1..c2]
map<char, int> components(int r1, int c1, int r2, int c2, int n, const vector<string>& grid) {
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    map<char, int> m;

    rep(i, r1, r2 + 1) {
        rep(j, c1, c2 + 1) {
            if (!visited[i][j]) {
                char c = ff(i, j, r1, c1, r2, c2, grid, visited);
                m[c]++;
            }
        }
    }

    return m;
}

// exactly two colors; one forms a single region, the other forms two or more
bool is_pcl(int r1, int c1, int r2, int c2, int n, const vector<string>& grid) {
    map<char, int> m = components(r1, c1, r2, c2, n, grid);
    if (sz(m) != 2) return false;
    int a = m.begin()->second;
    int b = next(m.begin())->second;
    return min(a, b) == 1 && max(a, b) >= 2;
}

void solve() {
    int n;
    cin >> n;

    vector<string> grid(n);

    rep(i, n) {
        cin >> grid[i];
    }

    // good[r1][c1][r2][c2]: rectangle satisfies the color/region property (ignoring maximality)
    static bool good[20][20][20][20];

    rep(r1, n) rep(c1, n) rep(r2, r1, n) rep(c2, c1, n) {
        good[r1][c1][r2][c2] = is_pcl(r1, c1, r2, c2, n, grid);
    }

    // a PCL is a good rectangle not strictly contained in any other good rectangle
    int ans = 0;
    rep(r1, n) rep(c1, n) rep(r2, r1, n) rep(c2, c1, n) {
        if (!good[r1][c1][r2][c2]) continue;

        bool contained = false;
        for (int a = 0; a <= r1 && !contained; a++) {
            for (int b = 0; b <= c1 && !contained; b++) {
                for (int c = r2; c < n && !contained; c++) {
                    for (int d = c2; d < n; d++) {
                        if (a == r1 && b == c1 && c == r2 && d == c2) continue;
                        if (good[a][b][c][d]) {
                            contained = true;
                            break;
                        }
                    }
                }
            }
        }

        if (!contained) ans++;
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