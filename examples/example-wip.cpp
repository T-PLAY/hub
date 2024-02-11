
#include <iostream>


#include <native/Native.hpp>




int main()
{
    hub::sensor::SensorSpec * sensorSpec = hub::native::createSensorSpec( "sensorName" );
    char buff[256];
    int strLen = 0;
    hub::native::sensorSpec_toString( sensorSpec, buff, &strLen );

    assert( strLen < 256 );
    std::cout << "buff = " << buff << std::endl;

    return 0;
}
