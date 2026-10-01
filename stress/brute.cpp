// Stress-test brute force (Ctrl+Alt+X): slow but obviously correct, same input/output as your solution.
// For problems with several valid answers, add stress/checker.cpp: it gets the paths of
// input, brute output and your output as argv[1..3] and must exit 0 when your output is valid.
#include <bits/stdc++.h>
using namespace std;

int main() {
    // Example: sum of the array. Replace with a brute force for the current problem.
    int n;
    cin >> n;
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;
        sum += x;
    }
    cout << sum << "\n";
}
