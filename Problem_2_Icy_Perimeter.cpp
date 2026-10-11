/**
 *    author:  Kshitish Madbhavi
 *    problem: Problem 2. Icy Perimeter
 *    source:  USACO - USACO 2019 January Contest, Silver
 *    url:     https://usaco.org/index.php?page=viewproblem2&cpid=895
 *    created: 2026-09-30 04:04:39
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
    string in = file(R"("fileName":"perimeter.in","type":"file")"), out = file(R"("fileName":"perimeter.out","type":"file")");
    if (!in.empty()) freopen(in.c_str(), "r", stdin);
    if (!out.empty()) freopen(out.c_str(), "w", stdout);
#endif
}


void solve() {
    int n;
    cin >> n;

    vector<string> grid(n);

    for (auto& s : grid) {
        cin >> s;
    }
    
    vector<tuple<int, int, int>> regions;  // (id, area, perimeter); regions.size() = number of components
    vector<vector<int>> region;            // region id per cell, -1 = not filled
    {
        const int H = (int)grid.size(), W = H ? (int)grid[0].size() : 0;  // rectangular grid; set by hand for C arrays
        const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
        auto open = [&](int r, int c) { return grid[r][c] == '#'; };  // which cells get filled
        // may two touching open cells share a region? (symmetric) e.g. [&](int r, int c, int nr, int nc) { return grid[r][c] == grid[nr][nc]; }
        auto same = [&](int, int, int, int) { return true; };
        region.assign(H, vector<int>(W, -1));
        queue<pair<int, int>> q;
        for (int sr = 0; sr < H; sr++) {
            for (int sc = 0; sc < W; sc++) {
                if (region[sr][sc] != -1 || !open(sr, sc)) continue;
                int id = (int)regions.size(), area = 0, perim = 0;
                region[sr][sc] = id;
                q.push({sr, sc});
                while (!q.empty()) {
                    auto [r, c] = q.front();
                    q.pop();
                    area++;
                    for (int k = 0; k < 4; k++) {
                        int nr = r + dr[k], nc = c + dc[k];
                        if (nr < 0 || nr >= H || nc < 0 || nc >= W || !open(nr, nc) || !same(r, c, nr, nc)) {
                            perim++;  // this side faces the border, a closed cell or another region (holes count too)
                            continue;
                        }
                        if (region[nr][nc] != -1) continue;
                        region[nr][nc] = id;
                        q.push({nr, nc});
                    }
                }
                regions.emplace_back(id, area, perim);
            }
        }
    }

    pair<int, int> ans; 
    ans = {0, 0};

    for (const auto& tup:regions) {
        if (ans.first == get<1>(tup)) chmin(ans.second, get<2>(tup));
        if (chmax(ans.first, get<1>(tup))) ans.second = get<2>(tup); 
    }

    cout << ans.first << " " << ans.second;
    

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
