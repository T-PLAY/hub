

//#include <string>
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <numeric>

//#include <SensorSpec.hpp>
//#include <Macros.hpp>

#include <Acquisition.hpp>

int main() {

#if CPLUSPLUS_VERSION == 20
    // todo change char ptr into std::array
    constexpr const unsigned char data[] { 0, 1, 2 };

    // std::pair is not constexpr :(
//    constexpr hub::SensorSpec::Resolution resolution = {{1}, hub::Format::BGR8};
//    constexpr hub::Measure measure { data, 4, { { 1 }, hub::Format::BGR8 } };
#endif
}