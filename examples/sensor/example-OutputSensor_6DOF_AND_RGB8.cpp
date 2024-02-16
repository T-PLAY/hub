
#include <thread>

#include <sensor/outputSensor/OutputSensor_RGB8.hpp>
#include <sensor/outputSensor/OutputSensor_Dof6.hpp>

int main() {

    hub::sensor::outputSensor::OutputSensor_Dof6 outputSensor_Dof6;
    outputSensor_Dof6.run();

    hub::sensor::outputSensor::OutputSensor_Dof6 outputSensor_RGB8;
    outputSensor_RGB8.run();

    return 0;
}
