// @name   Grid BFS (shortest path on a grid)
// @prefix cpgridbfs
// @key    t g
// @desc   [inside solve] needs n, m, grid. Starts from every 'S' (multi-source), '#' is a wall.
// @usage  // after it: dist[r][c] = steps from the nearest 'S' (-1 = unreachable)
// @complexity O(nm)
#include <bits/stdc++.h>
using namespace std;

void context(int n, int m, vector<string>& grid) {
    // @begin
    const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    vector<vector<int>> dist(n, vector<int>(m, -1));  // -1 = unreachable
    queue<pair<int, int>> q;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < m; c++) {
            if (grid[r][c] == 'S') {
                dist[r][c] = 0;
                q.push({r, c});
            }
        }
    }
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (grid[nr][nc] == '#' || dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    // @end
}
