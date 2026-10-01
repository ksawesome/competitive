// @name   Interactive problem helpers
// @prefix cpinteractive
// @key    t shift+i
// @desc   [inside solve] ask(i, j) prints "? i j", flushes, reads the reply (exits on -1); answer(x) prints "! x".
// @usage  int r = ask(i, j);
// @usage  answer(x);
#include <bits/stdc++.h>
using namespace std;

void context() {
    // @begin
    auto ask = [&](int i, int j) {
        cout << "? " << i << ' ' << j << endl;  // endl flushes: required for interactive judges
        int res;
        cin >> res;
        if (res == -1) exit(0);  // judge says wrong / too many queries
        return res;
    };
    auto answer = [&](int x) { cout << "! " << x << endl; };
    // @end
    (void)ask, (void)answer;
}
