
#include <thread>

#include <sensor/outputSensor/OutputSensor_Dof6.hpp>

int main() {

    hub::sensor::outputSensor::OutputSensor_Dof6 outputSensorDof6;
    outputSensorDof6.run();

    return 0;
}
