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

bool bfs(int start, int power, vector<bool>& visited,
         vector<vector<int>>& dist) {

    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < visited.size(); i++) {
            if (!visited[i] && dist[u][i] <= power) {
                visited[i] = true;
                q.push(i);
            }
        }
    }

    for (bool x : visited) {
        if (!x) return false;
    }

    return true;
}

void solve() {
    int n;
    cin >> n;

    vector<pair<int, int>> grid(n);

    rep(i, n) {
        cin >> grid[i].first >> grid[i].second;
    }

    vector<vector<int>> dist(n, vector<int>(n, INF));

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {

            int dx = grid[i].first - grid[j].first;
            int dy = grid[i].second - grid[j].second;

            int temp = dx * dx + dy * dy;

            dist[i][j] = temp;
            dist[j][i] = temp;
        }
    }

    int l = 0;
    int r = 1250000000LL; // 25000^2 + 25000^2

    while (l < r) {
        int mid = l + (r - l) / 2;

        vector<bool> visited(n, false);

        if (bfs(0, mid, visited, dist)) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    cout << l << '\n';
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    freopen("moocast.in", "r", stdin);
    freopen("moocast.out", "w", stdout);

    solve();
}