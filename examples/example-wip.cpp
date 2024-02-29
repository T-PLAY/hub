
#include <iostream>


#include <native/Native.hpp>




int main()
{
    for (int i = 0; i < 50; ++i) {
        std::cout << std::to_string(i) << ":\033[" << std::to_string(i) << "mhello\033[0m" << std::endl;
    }
    hub::sensor::SensorSpec * sensorSpec = hub::native::createSensorSpec( "sensorName" );
    char buff[256];
    int strLen = 0;
    hub::native::sensorSpec_toString( sensorSpec, buff, &strLen );

    assert( strLen < 256 );
    std::cout << "buff = " << buff << std::endl;

    return 0;
}
