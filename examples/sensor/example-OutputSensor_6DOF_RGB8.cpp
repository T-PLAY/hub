
#include <thread>

#include <sensor/outputSensor/OutputSensor_Dof6_RGB8.hpp>

int main() {

    hub::sensor::outputSensor::OutputSensor_Dof6_RGB8 outputSensor_Dof6_RGB8;
    outputSensor_Dof6_RGB8.run();

    return 0;
}
