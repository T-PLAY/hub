
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

    return 0;
}
