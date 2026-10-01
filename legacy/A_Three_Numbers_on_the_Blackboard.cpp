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

int read() {
    int x;
    cin >> x;
    return x;
}

void solve() {
    	int n;
        cin >> n;

        rep (i, n) {
            vector<int> a;
		    a.emplace_back(read());
		    a.emplace_back(read());
		    a.emplace_back(read());
		    sort(a.begin(), a.end());
		    int ans = a[2] - a[0];
		    ans = min(ans, a[1]);
            cout << ans << "\n";
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
