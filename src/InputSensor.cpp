#include "InputSensor.hpp"

//#define DEBUG_STREAM

namespace hub {

Acquisition InputSensor::getAcquisition() const {

    Acquisition acq = m_interface.getAcquisition(m_spec.m_acquisitionSize);

    assert(acq.getSize() == m_spec.m_acquisitionSize);
    const auto & resolutions = m_spec.m_resolutions;
    const auto & measures = acq.getMeasures();
    assert(resolutions.size() == measures.size());
    for (int i = 0; i <resolutions.size(); ++i) {
        assert(SensorSpec::computeAcquisitionSize(resolutions.at(i)) == measures.at(i).m_size);
    }

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
