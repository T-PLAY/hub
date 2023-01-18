

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <Macros.hpp>
#include <SensorSpec.hpp>

// #if ( __cplusplus >= 202001L )
// #else
// #    error "only C++20 accepted"
// #endif

int main() {

    hub::SensorSpec::MetaData metaData;
    hub::Format format = hub::Format::BGR8;
    hub::Dims dims     = { 1 };
    hub::Resolutions resolutions { { dims, format } };
    hub::SensorSpec sensorSpec { "sensorName", { { dims, hub::Format::BGR8 } }, metaData };
    hub::SensorSpec sensorSpec2 {
        "sensorName", { { { 1 }, hub::Format::BGR8 }, { { 1, 2, 3 }, format } }, metaData };
    hub::SensorSpec sensorSpec3 { "sensorName", {}, metaData };
    //    static_assert(sensorSpec3.getAcquisitionSize() == 0);

#if CPLUSPLUS_VERSION == 20
    using namespace std::string_literals;
    //    constexpr auto sensorName = "sensorName"s;
    constexpr std::string_view sensorName2 = "sensorName";
    //    constexpr std::string sensorName2 = "sensorName";
    //    constexpr std::vector<std::string> myStringVec{"Stroustrup"s, "Vandevoorde"s,
    //                                                      "Sutter"s, "Josuttis"s, "Wong"s };
    //    constexpr auto sensorSpec4 = hub::SensorSpec(sensorName, {}, {});
    //    constexpr auto sensorSpec4 = hub::SensorSpec("sensorName"s, {}, {});
    //    constexpr std::vector<int> vec(2, 0) = {0, 2};
    using namespace std;
    constexpr std::array<int, 2> dims2 = { 2, 4 };
    //    constexpr auto resolution = {dims2, hub::Format::BGR8};
    //    constexpr hub::Dims dims3 = const_cast<const hub::Dims>({2, 4});
    //    constexpr auto resolutions2 = hub::Resolutions{{}, hub::Format::BGR8};
    //    constexpr hub::Dims dims2 = {2, 4};
    //    constexpr hub::Resolutions resolutions2 = {{5}, hub::Format::BGR8};
    //    constexpr std::vector<int> dims3 = const_cast<std::vector<int>>({1, 2});

    constexpr hub::SensorSpec::MetaData metaData2 = {};
    constexpr auto sensorSpec4                    = hub::SensorSpec( sensorName2, {}, metaData2 );
    static_assert( sensorSpec4.getSensorName() == "sensorName" );
    static_assert( sensorSpec4.getResolutions().size() == 0 );
    static_assert( sensorSpec4.getMetaData().size() == 0 );
    static_assert( sensorSpec4.getAcquisitionSize() == 0 );

    constexpr auto sensorSpec5 = hub::SensorSpec( "sensorName" );
    static_assert( sensorSpec5.getSensorName() == "sensorName" );
    static_assert( sensorSpec5.getAcquisitionSize() == 0 );
#endif
}
