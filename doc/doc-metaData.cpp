
#include <iostream>

#include <SensorSpec.hpp>

int main() {

    hub::MetaData metaData;
    metaData["hello"] = 0;

    hub::Any any( "hello" );

    const auto& value = any.getConstCharPtr();
    std::cout << value << std::endl;

    return 0;
}
