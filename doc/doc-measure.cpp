

// #include <string>
// #include <iostream>
// #include <algorithm>
// #include <vector>
// #include <numeric>

// #include <SensorSpec.hpp>
// #include <Macros.hpp>

#include <sensor/Acquisition.hpp>
#include <istream>
#include <ostream>
#include <streambuf>

/// \file

int main() {

#if CPLUSPLUS_VERSION == 20
    // todo change char ptr into std::array
    constexpr const unsigned char data[] { 0, 1, 2 };

    // std::pair is not constexpr :(
#endif

    return 0;
}
