
#include <iostream>

#include <core/MetaData.hpp>

/// \file
int main() {

    /// startConstruction
    /// metaData
    /// endConstruction
    hub::MetaData metaData;
    metaData["hello"] = 0;

    hub::Any any( "hello" );

    auto value = any.get<const char*>();
    std::cout << value << std::endl;

    std::vector<hub::format::Vec3> points;
    // hub::format::Vec3 vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < 2000; ++i) {
        points.emplace_back(i, i, i);
    }
    std::cout << "points: " << points << std::endl;

    metaData["poins"] = points;
    std::cout << "metaData: " << metaData << std::endl;


    return 0;
}
