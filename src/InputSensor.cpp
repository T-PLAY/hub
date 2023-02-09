#include "InputSensor.hpp"

// #define DEBUG_STREAM

namespace hub {

Acquisition InputSensor::getAcquisition() const {

    //    Acquisition && acq = m_interface.getAcquisition( static_cast<int>(
    //    m_spec.getAcquisitionSize() ) );
    Acquisition&& acq = m_interface.getAcquisition( m_spec );

    //    if (acq.m_start == -1 && acq.m_end == -1) {
    //        return acq;
    //    }

    assert( acq.getSize() == m_spec.getAcquisitionSize() );
    const auto& resolutions = m_spec.getResolutions();
    const auto& measures          = acq.m_measures;
    assert( resolutions.size() == measures.size() );
    assert( resolutions.size() > 0 );
    for ( size_t i = 0; i < resolutions.size(); ++i ) {
        assert( computeAcquisitionSize( resolutions.at( i ) ) == measures.at( i ).m_size );
        //        measures.at( i ).m_resolution = resolutions.at( i );
        assert( measures.at( i ).m_resolution == resolutions.at( i ) );
        assert( !measures.at( i ).m_resolution.first.empty() );
        assert( measures.at( i ).m_resolution.second != Format::NONE );
    }

#ifdef DEBUG_STREAM
    Acquisition acquisition( start, end, data, spec.m_acquisitonSize );
    std::cout << "[InputSensor] read acq :  " << acquisition << std::endl;
#endif

    return std::move( acq );
}

std::vector<Acquisition> InputSensor::getAllAcquisitions() {
    std::vector<Acquisition> acqs;

    try {
        int nReadAcqs = 0;
        //        while ( true ) {
        while ( !m_interface.isEnd() ) {
            acqs.emplace_back( getAcquisition() );
            ++nReadAcqs;
        }
    }
    //    catch ( Sensor::exception& e ) {
    //        std::cout << "[InputSensor] catch sensor exception : " << e.what() << std::endl;
    //        throw e;
    //    }
    catch ( std::exception& e ) {
        std::cout << "[InputSensor] catch exception : " << e.what() << std::endl;
        throw;
    }

    return acqs;
}

} // namespace hub
