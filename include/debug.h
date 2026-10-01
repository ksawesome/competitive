// Local-only debug printer. Active only when compiled with -DLOCAL (CPH, F5 and the
// stress tester all pass it); on the judge `dbg(...)` expands to nothing, so you never
// have to delete debug lines before submitting.
//
//   dbg(n, v, mp);    ->  [L42] n = 3 | v = {1, 2, 3} | mp = {(1, 2), (5, 7)}
//   dbg();            ->  [L43] reached
//
// Prints to stderr: CPH shows it in the test's "Standard Error" box (cph.general.ignoreSTDERROR is
// on, so debug output never turns a correct answer into a failure). Handles numbers, bool,
// char, strings, pairs, tuples, any container (nested too), stack/queue/priority_queue,
// bitset and __int128.
#pragma once
#pragma GCC system_header
#include <bits/stdc++.h>

namespace dbg_impl {

template <class T, class = void> struct iterable : std::false_type {};
template <class T>
struct iterable<T, std::void_t<decltype(std::begin(std::declval<const T&>())),
                               decltype(std::end(std::declval<const T&>()))>> : std::true_type {};

template <class T, class = void> struct has_top : std::false_type {};  // stack, priority_queue
template <class T>
struct has_top<T, std::void_t<decltype(std::declval<T&>().top()), decltype(std::declval<T&>().pop())>>
    : std::true_type {};

template <class T, class = void> struct has_front_pop : std::false_type {};  // queue
template <class T>
struct has_front_pop<T, std::void_t<decltype(std::declval<T&>().front()), decltype(std::declval<T&>().pop())>>
    : std::true_type {};

template <class T> struct is_pair : std::false_type {};
template <class A, class B> struct is_pair<std::pair<A, B>> : std::true_type {};
template <class T> struct is_tuple : std::false_type {};
template <class... Ts> struct is_tuple<std::tuple<Ts...>> : std::true_type {};

inline void print_i128(std::ostream& os, unsigned __int128 x, bool neg) {
    std::string s;
    do { s += char('0' + int(x % 10)); x /= 10; } while (x);
    if (neg) s += '-';
    std::reverse(s.begin(), s.end());
    os << s;
}

template <class T>
void print(std::ostream& os, const T& x) {
    using U = std::decay_t<T>;
    if constexpr (std::is_same_v<U, bool>) {
        os << (x ? "true" : "false");
    } else if constexpr (std::is_same_v<U, char>) {
        os << '\'' << x << '\'';
    } else if constexpr (std::is_same_v<U, std::string> || std::is_same_v<U, std::string_view> ||
                         std::is_same_v<U, const char*> || std::is_same_v<U, char*>) {
        os << '"' << x << '"';
    } else if constexpr (std::is_same_v<U, __int128>) {
        print_i128(os, x < 0 ? -(unsigned __int128)x : (unsigned __int128)x, x < 0);
    } else if constexpr (std::is_same_v<U, unsigned __int128>) {
        print_i128(os, x, false);
    } else if constexpr (std::is_arithmetic_v<U>) {
        os << x;
    } else if constexpr (std::is_same_v<U, std::vector<bool>::reference>) {
        os << (bool(x) ? "true" : "false");
    } else if constexpr (is_pair<U>::value) {
        os << '(';
        print(os, x.first);
        os << ", ";
        print(os, x.second);
        os << ')';
    } else if constexpr (is_tuple<U>::value) {
        os << '(';
        std::apply([&](const auto&... e) {
            int i = 0;
            ((os << (i++ ? ", " : ""), print(os, e)), ...);
        }, x);
        os << ')';
    } else if constexpr (iterable<U>::value) {
        os << '{';
        bool first = true;
        for (const auto& e : x) {
            if (!first) os << ", ";
            first = false;
            print(os, e);
        }
        os << '}';
    } else if constexpr (has_top<U>::value || has_front_pop<U>::value) {
        U c = x;  // adapters can't be iterated: print a copy front-to-back
        os << '{';
        for (bool first = true; !c.empty(); first = false) {
            if (!first) os << ", ";
            if constexpr (has_top<U>::value) print(os, c.top());
            else print(os, c.front());
            c.pop();
        }
        os << '}';
    } else {
        os << x;  // bitset and anything else with operator<<
    }
}

// Splits "a, f(x, y), v[i]" into {"a", "f(x, y)", "v[i]"} at top-level commas.
inline std::vector<std::string> split_names(const std::string& s) {
    std::vector<std::string> out(1);
    int depth = 0;
    char quote = 0;
    for (size_t i = 0; i < s.size(); i++) {
        char ch = s[i];
        if (quote) {
            if (ch == '\\' && i + 1 < s.size()) out.back() += s[i++];
            else if (ch == quote) quote = 0;
        } else if (ch == '"' || ch == '\'') {
            quote = ch;
        } else if (ch == '(' || ch == '[' || ch == '{') {
            depth++;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            depth--;
        } else if (ch == ',' && depth == 0) {
            out.emplace_back();
            continue;
        }
        if (!(ch == ' ' && out.back().empty())) out.back() += ch;
    }
    return out;
}

// dbg(a, b) becomes Printer{line, "a, b"}(a, b): calling the printer handles zero arguments
// without the C++20 __VA_OPT__, so this header is plain C++17.
struct Printer {
    int line;
    const char* names;
    template <class... Ts>
    void operator()(const Ts&... xs) const {
        std::cerr << "[L" << line << "] ";
        if constexpr (sizeof...(Ts) == 0) {
            std::cerr << "reached";
        } else {
            std::vector<std::string> ns = split_names(names);
            size_t i = 0;
            ((std::cerr << (i ? " | " : "") << (i < ns.size() ? ns[i] : "?") << " = ", print(std::cerr, xs), i++), ...);
        }
        std::cerr << std::endl;  // flush now, so output survives a crash on the next line
    }
};

}  // namespace dbg_impl

#define dbg(...) dbg_impl::Printer{__LINE__, #__VA_ARGS__}(__VA_ARGS__)
