#include <bits/stdc++.h>
using namespace std;

using ll = long long;

using pii = pair<int, int>;
using pll = pair<ll, ll>;

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

void dfs(int start, vector<vector<pair<int,int>>>& adj, vector<int>& color, bool& impossible) {
    for (auto &[to, sameType] : adj[start]) {
        int expected = sameType ? color[start] : (1 - color[start]);
        if (color[to] == -1) {
            color[to] = expected;
            dfs(to, adj, color, impossible);
        } else if (color[to] != expected) {
            impossible = true;
        }
    }
}

void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int,int>>> adj(n);

    rep(i, m) {
        int u, v;
        char c;

        cin >> c >> u >> v;
        u--;
        v--;

        int sameType = (c == 'S') ? 1 : 0;
        adj[u].push_back({v, sameType});
        adj[v].push_back({u, sameType});
    }

    int k = 0;
    vector<int> color(n, -1);
    bool impossible = false;

    rep(i, n) {
        if (color[i] == -1) {
            k++;
            color[i] = 1;
            dfs(i, adj, color, impossible);
        }
    }

    if (impossible) {
        cout << 0 << "\n";
    } else {
        cout << 1;
        rep(i, k) cout << 0;
        cout << "\n";
    }
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    freopen("revegetate.in", "r", stdin);
    freopen("revegetate.out", "w", stdout);

    int t = 1;
    // cin >> t;
    while (t--) solve();
}