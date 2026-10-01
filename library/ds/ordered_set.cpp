// @name   Ordered set (PBDS order statistics)
// @prefix cposet
// @key    t shift+o
// @desc   [global] ordered_set<int> s; s.find_by_order(k) = k-th smallest (0-based), s.order_of_key(x) = # elements < x.
// @usage  ordered_set<int> s;
// @usage  s.insert(x); *s.find_by_order(k); s.order_of_key(x);
// @complexity O(log n) per operation
#include <bits/stdc++.h>
using namespace std;

// @begin
#pragma push_macro("int")  // PBDS headers break under `#define int long long`: include them with it off
#undef int
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#pragma pop_macro("int")
template <class T>
using ordered_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, less<T>, __gnu_pbds::rb_tree_tag,
                                     __gnu_pbds::tree_order_statistics_node_update>;
// multiset: store pair<T, int>{value, unique_id}
// @end
