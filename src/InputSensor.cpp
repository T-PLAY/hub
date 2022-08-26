#include "InputSensor.hpp"

//#define DEBUG_STREAM

namespace hub {

Acquisition InputSensor::getAcquisition() const {

    Acquisition acq = m_interface.getAcquisition(m_spec.m_acquisitionSize);

#ifdef DEBUG_STREAM
    Acquisition acquisition( start, end, data, spec.m_acquisitonSize );
    std::cout << "[InputSensor] read acq :  " << acquisition << std::endl;
#endif

    return acq;
}

std::vector<Acquisition> InputSensor::getAllAcquisitions() {
    std::vector<Acquisition> acqs;

    try {
        int nReadAcqs = 0;
        while ( true ) {
            acqs.emplace_back( getAcquisition() );
            ++nReadAcqs;
        }
    }
    catch ( Sensor::exception& e ) {
        std::cout << "[InputSensor] catch sensor exception : " << e.what() << std::endl;
        throw;
    }
    catch ( std::exception& e ) {
        std::cout << "[InputSensor] catch exception : " << e.what() << std::endl;
    }

    return acqs;
}

} // namespace hub
