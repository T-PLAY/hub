

// #include <string>
// #include <iostream>
// #include <algorithm>
// #include <vector>
// #include <numeric>

// #include <SensorSpec.hpp>
// #include <Macros.hpp>

#include <Acquisition.hpp>
#include <istream>
#include <ostream>
#include <streambuf>

/// \file

int main() {

    //    std::basic_ostream<int> out;

    //    int buff[256];
    //    std::basic_istream<int> input();

#if CPLUSPLUS_VERSION == 20
    // todo change char ptr into std::array
    constexpr const unsigned char data[] { 0, 1, 2 };

    // std::pair is not constexpr :(
//    constexpr hub::SensorSpec::Resolution resolution = {{1}, hub::Format::BGR8};
//    constexpr hub::Measure measure { data, 4, { { 1 }, hub::Format::BGR8 } };
#endif

    return 0;
}
