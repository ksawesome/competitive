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

int n, m;
vector<vector<pair<int,int>>> adj;
vector<int> sign_, off_, visited;
bool feasible = true;

void solve() {
    cin >> n >> m;
    adj.assign(n+1, {});
    rep(i, m) {
        int a, b, c;
        cin >> a >> b >> c;
        adj[a].push_back({b, c});
        adj[b].push_back({a, c});
    }

    visited.assign(n+1, 0);
    sign_.assign(n+1, 0);
    off_.assign(n+1, 0);
    vector<double> val(n+1, 0.0);

    for (int s = 1; s <= n && feasible; s++) {
        if (visited[s]) continue;

        vector<int> comp;
        bool xPinned = false;
        double pinnedX = 0.0;

        queue<int> q;
        q.push(s);
        visited[s] = 1;
        sign_[s] = 1;
        off_[s] = 0;
        comp.push_back(s);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto x : adj[u]) {
                int v = x.first;
                int c = x.second;
                int newSign = -sign_[u];
                int newOff = c - off_[u];

                if (!visited[v]) {
                    visited[v] = 1;
                    sign_[v] = newSign;
                    off_[v] = newOff;
                    comp.push_back(v);
                    q.push(v);
                } else {
                    if (sign_[v] == newSign) {
                        if (off_[v] != newOff) { feasible = false; }
                    } else {
                        double xv = (double)(newOff - off_[v]) / (double)(sign_[v] - newSign);
                        if (xPinned) {
                            if (fabs(xv - pinnedX) > 1e-6) feasible = false;
                        } else {
                            xPinned = true;
                            pinnedX = xv;
                        }
                    }
                }
            }
            if (!feasible) break;
        }
        if (!feasible) break;

        double xv;
        if (xPinned) {
            xv = pinnedX;
        } else {
            auto cost = [&](double xx) -> double {
                double s2 = 0;
                for (int node : comp) s2 += fabs(sign_[node]*xx + off_[node]);
                return s2;
            };
            double maxOff = 0;
            for (int node : comp) maxOff = max(maxOff, fabs((double)off_[node]));
            double lo = -maxOff - 5, hi = maxOff + 5;

            rep(iter, 100) {
                double m1 = lo + (hi-lo)/3;
                double m2 = hi - (hi-lo)/3;
                if (cost(m1) < cost(m2)) hi = m2; else lo = m1;
            }
            xv = (lo+hi)/2;
        }

        for (int node : comp) val[node] = sign_[node]*xv + off_[node];
    }

    if (!feasible) {
        cout << "NO\n";
        return;
    }

    cout << "YES\n";
    cout << fixed << setprecision(6);
    for (int i = 1; i <= n; i++) cout << val[i] << " \n"[i==n];
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