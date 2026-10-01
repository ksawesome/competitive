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

void dfs(int start, vector<vector<int>> &adj, vector<bool> &visited)
{
    visited[start] = true;

    for (auto x : adj[start])
    {
        if (!visited[x])
            dfs(x, adj, visited);
    }
}

void solve()
{
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_f(n);
    vector<vector<int>> adj_b(n);

    rep(i, m)
    {
        int a, b;
        cin >> a >> b;

        adj_f[--a].push_back(--b);
        adj_b[b].push_back(a);
    }

    vector<bool> visited_f(n, false);
    vector<bool> visited_b(n, false);

    dfs(0, adj_f, visited_f);
    dfs(0, adj_b, visited_b);

    bool flag_f = true;
    bool flag_b = true;

    for (int i = 0; i < n; i++)
    {
        if (!visited_f[i])
        {
            flag_f = false;
            break;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (!visited_b[i])
        {
            flag_b = false;
            break;
        }
    }

    if (flag_f && flag_b)
    {
        cout << "YES\n";
    }
    else
    {
        cout << "NO\n";

        if (!flag_f)
        {
            for (int i = 0; i < n; i++)
            {
                if (!visited_f[i])
                {
                    cout << 1 << " " << i + 1 << "\n";
                    break;
                }
            }
        }
        else
        {
            for (int i = 0; i < n; i++)
            {
                if (!visited_b[i])
                {
                    cout << i + 1 << " " << 1 << "\n";
                    break;
                }
            }
        }
    }
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // freopen(".in", "r", stdin);
    // freopen(".out", "w", stdout);

    int t = 1;
    // cin >> t;
    while (t--)
        solve();
}
