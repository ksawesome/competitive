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

void bfs(int start, int c, vector<vector<int>>& adj, vector<int>& color) {
    queue<int> q;
    q.push(start);
    color[start] = c;

    while (!q.empty()) {
        auto temp = q.front();
        q.pop();
        for (auto x : adj[temp]) {
            if (color[x] == 0) {
                q.push(x);
                color[x] = -color[temp];
            }

        }

    }
}

void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n);

    rep(i, m) {
        int u, v;
        cin >> u >> v;
        adj[--u].push_back(--v);
        adj[v].push_back(u);
    }

    vector<int> color(n, 0);

    bfs(0, 1, adj, color);

    pair<int, int> cnt = {0, 0};

    rep(i, n) {
        if (color[i] == 1) cnt.first++;
        else cnt.second++;
    }

    int ans = 0;

    if (cnt.first >= cnt.second) {
    ans = cnt.second;
    cout << ans << "\n";
    int val = 0;
    for (auto x : color) {
        if (x == -1) cout << (val + 1) << " ";
        val++;
    }
} else {
    ans = cnt.first;
    cout << ans << "\n";
    int val = 0;
    for (auto x : color) {
        if (x == 1) cout << (val + 1) << " ";
        val++;
    }
}
cout << "\n";
}



signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    //freopen(".in", "r", stdin);
    //freopen(".out", "w", stdout);

    int t = 1;
    cin >> t;
    while (t--) solve();
}
