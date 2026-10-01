// @name   Flood fill (grid regions)
// @prefix cpflood
// @key    t f
// @desc   [inside solve] needs n, m, grid (vector<string>). Iterative, so big grids can't overflow the stack.
// @usage  // after it: region[r][c] = region id (-1 = not open), region_size[id]
// @usage  // change open(r, c) to choose which cells get filled
// @complexity O(nm)
#include <bits/stdc++.h>
using namespace std;

void context(int n, int m, vector<string>& grid) {
    // @begin
    const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    auto open = [&](int r, int c) { return grid[r][c] == '.'; };  // which cells get filled
    vector<vector<int>> region(n, vector<int>(m, -1));  // region id per cell, -1 = not filled
    vector<int> region_size;
    for (int sr = 0; sr < n; sr++) {
        for (int sc = 0; sc < m; sc++) {
            if (!open(sr, sc) || region[sr][sc] != -1) continue;
            int id = region_size.size();
            region_size.push_back(0);
            queue<pair<int, int>> q;
            q.push({sr, sc});
            region[sr][sc] = id;
            while (!q.empty()) {
                auto [r, c] = q.front();
                q.pop();
                region_size[id]++;
                for (int k = 0; k < 4; k++) {
                    int nr = r + dr[k], nc = c + dc[k];
                    if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
                    if (!open(nr, nc) || region[nr][nc] != -1) continue;
                    region[nr][nc] = id;
                    q.push({nr, nc});
                }
            }
        }
    }
    // number of regions = region_size.size()
    // @end
}
