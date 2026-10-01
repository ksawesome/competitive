#include <bits/stdc++.h>
using namespace std;

using ll = long long;

using pii = pair<int, int>;
using pll = pair<ll, ll>;

using minpqi = priority_queue<int, vector<int>, greater<int>>;
using minpql = priority_queue<ll, vector<ll>, greater<ll>>;
using minpqpii = priority_queue<pii, vector<pii>, greater<pii>>;
using minpqpll = priority_queue<pll, vector<pll>, greater<pll>>;

const ll INF = 4e18;

#define all(x) (x).begin(), (x).end()
#define rep(i, n) for (int i = 0; i < n; i++)
#define int long long

template <typename T>
bool chmax(T &a, const T &b) {
    if (b > a) {
        a = b;
        return true;
    }
    return false;
}

template <typename T>
bool chmin(T &a, const T &b) {
    if (b < a) {
        a = b;
        return true;
    }
    return false;
}

void floodfill(int x, int y, int area, int perimeter, vector<vector<int>>& grid, vector<vector<bool>>& visited) {
    visited[x][y] = true;
    int per = 4;

    if (x - 1 < 0 || visited[x - 1][y]) {
        per--;
    } else if (x + 1 < 0 || visited[x - 1][y])
}

void solve() {
    int n;
    cin >> n;

    vector<vector<int>> grid(n, vector<int>(n, 0));

    rep(i, n) {
        rep(j, n) {
            char temp;
            cin >> temp;

            if (temp == '#') grid[i][j] = 1;
        }
    }


}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    //freopen(".in", "r", stdin);
    //freopen(".out", "w", stdout);

    int t = 1;
    // cin >> t;
    while (t--) solve();
}
