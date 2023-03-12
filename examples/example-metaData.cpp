
#include <iostream>

#include <SensorSpec.hpp>

int main() {

    hub::SensorSpec::MetaData metaData;
    metaData["hello"] = 0;

    hub::Any any("hello");

//    auto value = any.get<int>();
    const auto & value = any.getConstCharPtr();
    std::cout << value << std::endl;


    return 0;
}
