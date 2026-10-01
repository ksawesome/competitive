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

void dfs(int start, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& instack, vector<bool>& incycle) {
    visited[start] = true;
    instack.push_back(start);

    for (auto x : adj[start]) {
        if (!visited[x]) {
            dfs(x, adj, visited, instack, incycle);
        } else if (find(all(instack), x) != instack.end()) {
            auto it = find(all(instack), x);
            while (it != instack.end()) {
                incycle[*it] = true;
                it++;
            }
        }
    }

    instack.pop_back();
}

void solve() {
    int n, k;
    cin >> n >> k;

    vector<vector<int>> adj(n);

    rep(i, k) {
        int a, b, s_a, s_b;
        cin >> a >> b >> s_a >> s_b;

        if (s_a > s_b) adj[--a].push_back(--b);
        else adj[--b].push_back(--a);
    }

    vector<bool> incycle(n, false);
    vector<bool> visited(n, false);
    vector<int> instack;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs(i, adj, visited, instack, incycle);
        }
    }

    int ans = 0;

    for (auto x : incycle) {
        if (x == true) ans++;
    }

    cout << ans;




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
