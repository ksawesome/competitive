// @name   Flood fill (grid regions: area + perimeter)
// @prefix cpflood
// @key    t f
// @desc   [inside solve] needs only grid (vector<string>, vector<vector<T>>, ...); size is read from it. Iterative, so big grids can't overflow the stack.
// @usage  // after it: regions[i] = (id, area, perimeter) with id == i, so regions.size() = number of components
// @usage  // region[r][c] = id of that cell's component (-1 = not filled)
// @usage  // change open(r, c) to choose which cells get filled; change same(...) to also split touching cells by value
// @complexity O(nm)
#include <bits/stdc++.h>
using namespace std;

void context(vector<string>& grid) {
    // @begin
    vector<tuple<int, int, int>> regions;  // (id, area, perimeter); regions.size() = number of components
    vector<vector<int>> region;            // region id per cell, -1 = not filled
    {
        const int H = (int)grid.size(), W = H ? (int)grid[0].size() : 0;  // rectangular grid; set by hand for C arrays
        const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
        auto open = [&](int r, int c) { return grid[r][c] == '.'; };  // which cells get filled
        // may two touching open cells share a region? (symmetric) e.g. [&](int r, int c, int nr, int nc) { return grid[r][c] == grid[nr][nc]; }
        auto same = [&](int, int, int, int) { return true; };
        region.assign(H, vector<int>(W, -1));
        queue<pair<int, int>> q;
        for (int sr = 0; sr < H; sr++) {
            for (int sc = 0; sc < W; sc++) {
                if (region[sr][sc] != -1 || !open(sr, sc)) continue;
                int id = (int)regions.size(), area = 0, perim = 0;
                region[sr][sc] = id;
                q.push({sr, sc});
                while (!q.empty()) {
                    auto [r, c] = q.front();
                    q.pop();
                    area++;
                    for (int k = 0; k < 4; k++) {
                        int nr = r + dr[k], nc = c + dc[k];
                        if (nr < 0 || nr >= H || nc < 0 || nc >= W || !open(nr, nc) || !same(r, c, nr, nc)) {
                            perim++;  // this side faces the border, a closed cell or another region (holes count too)
                            continue;
                        }
                        if (region[nr][nc] != -1) continue;
                        region[nr][nc] = id;
                        q.push({nr, nc});
                    }
                }
                regions.emplace_back(id, area, perim);
            }
        }
    }
    // @end
}