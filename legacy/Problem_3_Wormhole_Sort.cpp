#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int main() {
	std::ifstream read("wormsort.in");
	int cow_num;
	int wormhole_num;
	read >> cow_num >> wormhole_num;

	vector<int> cows(cow_num);
	for (int &c : cows) {
		read >> c;
		c--;  // make the cows 0-indexed
	}

	int max_width = 0;
	vector<vector<std::pair<int, int>>> neighbors(cow_num);
	for (int w = 0; w < wormhole_num; w++) {
		int c1, c2, width;
		read >> c1 >> c2 >> width;
		c1--;
		c2--;
		neighbors[c1].push_back({c2, width});
		neighbors[c2].push_back({c1, width});
		max_width = std::max(max_width, width);
	}

	int lo = 0;
	int hi = max_width + 1;
	int valid = -1;
	while (lo <= hi) {
		int mid = (lo + hi) / 2;

		vector<int> component(cow_num, -1);
		int curr_comp = 0;
		for (int c = 0; c < cow_num; c++) {
			if (component[c] != -1) { continue; }
			vector<int> frontier{c};
			while (!frontier.empty()) {
				int curr = frontier.back();
				frontier.pop_back();
				component[curr] = curr_comp;
				for (const auto &[n, w] : neighbors[curr]) {
					if (component[n] == -1 && w >= mid) { frontier.push_back(n); }
				}
			}
			curr_comp++;
		}

		bool sortable = true;
		for (int c = 0; c < cow_num; c++) {
			if (component[c] != component[cows[c]]) {
				sortable = false;
				break;
			}
		}

		if (sortable) {
			valid = mid;
			lo = mid + 1;
		} else {
			hi = mid - 1;
		}
	}

	std::ofstream("wormsort.out") << (valid == max_width + 1 ? -1 : valid) << endl;
}