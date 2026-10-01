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

void solve() {
    int n;
    cin >> n;

    vector<int> bfs(n);

    rep(i, n) {
        int temp;
        cin >> temp;
        bfs[i] = temp - 1;
    }

    vector<int> dfs(n);

    rep(i, n) {
        int temp;
        cin >> temp;
        dfs[i] = temp - 1;
    }

    vector<set<int>> adj(n);
    int ans = 0;

    if (n == 1) {
    cout << 0 << endl;
    return;
}

    if (bfs[1] != dfs[1]) cout << -1;
    else {
        for (auto x : bfs) {
            if (x != 0) {
                adj[0].insert(x);
                ans++;
            }
        }

        for (int i = 1; i < n; i++) {
    if (adj[dfs[i - 1]].insert(dfs[i]).second) {
        ans++;
    }
}

        cout << ans << endl;

        for (int i = 0; i < n; i++) {
            for (auto x : adj[i]) {
                cout << i + 1 << " " << x + 1 << endl;
            }
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
