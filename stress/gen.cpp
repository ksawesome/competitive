// Stress-test generator (Ctrl+Alt+X): print ONE random test in the problem's input format.
// argv[1] is the test number and seeds the RNG, so every failing test is reproducible.
// Keep tests tiny (n <= 8 or so): the brute force stays fast and failures are easy to read.
#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng;
long long rnd(long long l, long long r) { return uniform_int_distribution<long long>(l, r)(rng); }

int main(int argc, char** argv) {
    rng.seed(argc > 1 ? atoll(argv[1]) : 1);

    // Example: an array. Replace with the current problem's input format.
    int n = rnd(1, 8);
    cout << n << "\n";
    for (int i = 0; i < n; i++) cout << rnd(-10, 10) << " \n"[i == n - 1];
}
