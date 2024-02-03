
#include "native_InputSensor.hpp"

#define HUB_CPP_SOURCE
#include "sensor/InputSensor.hpp"

namespace hub {
namespace native {

sensor::InputSensor* createInputSensor( const char* streamName, const char* ipv4, int port ) {
    std::cout << "[Native] createInputSensor( " << streamName << ")" << std::endl;
    sensor::InputSensor* inputSensor = nullptr;
    try {
        inputSensor = new sensor::InputSensor( input::InputStream( streamName, port, ipv4 ) );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] createInputSensor : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return inputSensor;
}

void freeInputSensor( sensor::InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );
    std::cout << "[Native] freeinputSensor( " << inputSensor << ")" << std::endl;
    delete inputSensor;
}

int getAcquisitionSize( const sensor::InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );
    return static_cast<int>( inputSensor->getSpec().getResolution().size() );
}

sensor::Acquisition* getAcquisition( sensor::InputSensor* inputSensor ) {
    assert( inputSensor != nullptr );

    std::cout << "[Native] getAcquisition( " << inputSensor << ")" << std::endl;
    sensor::Acquisition* ret = nullptr;

    auto acq = inputSensor->acqMsg();
    try {
        *inputSensor >> acq;
        std::cout << "[Native] get acq : " << acq << std::endl;
        ret = new sensor::Acquisition( acq.clone() );
    }
    catch ( std::exception& e ) {
        std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
        return nullptr;
    }
    return ret;
}

} // namespace native
} // namespace hub
