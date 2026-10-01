// @name   Simulated annealing skeleton
// @prefix cpanneal
// @key    u z
// @desc   [global] anneal(s, energy, mutate, seconds, t0, t1): minimize energy(state) by random local moves; worse moves are accepted with probability exp(-delta / T), T cooling from t0 to t1 geometrically. Returns {best state, best energy}. For heuristic / output-only problems.
// @usage  auto [best, e] = anneal(start, [&](const State& s) { return cost(s); }, [&](State s, mt19937_64& rng) { /* small change */ return s; }, 1.8, 10.0, 0.01);
// @complexity (seconds) x (cost of energy + mutate)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// @begin
template <class State, class Energy, class Mutate>
pair<State, double> anneal(State s, Energy energy, Mutate mutate, double seconds, double t0, double t1) {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> unif(0, 1);
    auto start = chrono::steady_clock::now();
    double e = energy(s), best_e = e, frac = 0;
    State best = s;
    for (long long iter = 0;; iter++) {
        if ((iter & 255) == 0) {  // reading the clock is slow: only every 256 moves
            frac = chrono::duration<double>(chrono::steady_clock::now() - start).count() / seconds;
            if (frac >= 1) break;
        }
        double T = t0 * pow(t1 / t0, frac);
        State cand = mutate(s, rng);
        double ce = energy(cand);
        if (ce <= e || unif(rng) < exp((e - ce) / T)) {
            s = move(cand), e = ce;
            if (e < best_e) best = s, best_e = e;
        }
    }
    return {best, best_e};
}
// @end
