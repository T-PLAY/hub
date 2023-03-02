
#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <IO/Interface.hpp>

TEST_CASE( "InputSensor test" ) {

    hub::io::CyclicBuff buff;
//    hub::io::Ram ram(buff);

//    hub::InputSensor inputSensor(hub::io::Ram(buff));

    hub::SensorSpec sensorSpec;
    hub::OutputSensor outputSensor(sensorSpec, hub::io::Ram(buff));

//    outputSensor << hub::Acquisition(0, 1);

    hub::InputSensor inputSensor = hub::InputSensor(hub::io::Ram(buff));
}
