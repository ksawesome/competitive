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

void bfs(int start, int color,
         vector<vector<int>>& adj,
         vector<int>& visited) {

    queue<int> q;
    q.push(start);
    visited[start] = color;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbour : adj[node]) {

            if (visited[neighbour] == 0) {
                visited[neighbour] = -visited[node];
                q.push(neighbour);
            }
            else if (visited[neighbour] == visited[node]) {
                return;
            }
        }
    }
}

void solve() {
    int n;
    cin >> n;

    vector<vector<int>> adj(n);

    rep(i, n - 1) {
        int u, v;
        cin >> u >> v;

        adj[--u].push_back(--v);
        adj[v].push_back(u);
    }

    vector<int> visited(n, 0);

    bfs(0, 1, adj, visited);

    pair<int, int> c;
    c.first = 0;
    c.second = 0;

    for (auto x : visited) {
        if (x == 1) c.first++;
        else c.second++;
    }

    cout << (c.first) * (c.second) - (c.first + c.second - 1);




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
