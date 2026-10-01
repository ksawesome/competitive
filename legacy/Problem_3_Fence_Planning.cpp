#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

string humanReadable(uint64_t bytes)
{
    const char* units[] = {
        "B", "KB", "MB", "GB", "TB",
        "PB", "EB", "ZB", "YB"
    };

    double size = static_cast<double>(bytes);
    int unit = 0;

    while (size >= 1024.0 && unit < 8) {
        size /= 1024.0;
        ++unit;
    }

    ostringstream out;
    out << fixed << setprecision(2)
        << size << ' ' << units[unit];

    return out.str();
}

int main()
{
    cpp_int x = 2;

    while (true)
    {
        // Double the number.
        x <<= 1;

        // Number of bits required to represent x.
        size_t bits = boost::multiprecision::msb(x) + 1;

        // Minimum number of bytes needed for the magnitude.
        uint64_t bytes = (bits + 7) / 8;

        cout << x << '\n';

        cout << "Memory for magnitude: "
             << humanReadable(bytes)
             << " (" << bytes << " bytes)\n";

        cout << "Bits: "
             << bits << '\n';

        cout << "Decimal digits: "
             << static_cast<size_t>(
                    bits * 0.30103 + 1
                )
             << '\n';

        cout << "----------------------------------------\n";

        this_thread::sleep_for(
            chrono::milliseconds(0)
        );
    }
}