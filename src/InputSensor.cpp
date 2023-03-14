#include "InputSensor.hpp"

// #define DEBUG_STREAM

namespace hub {

Acquisition InputSensor::getAcquisition() const {

    Acquisition acq = m_input->getAcquisition( m_spec );

    assert( !acq.hasFixedSize() || acq.getSize() == m_spec.getAcquisitionSize() );
    const auto& resolutions = m_spec.getResolutions();
    const auto& measures    = acq.m_measures;
    assert( resolutions.size() == measures.size() );
    assert( resolutions.size() > 0 );
    for ( size_t i = 0; i < resolutions.size(); ++i ) {
        assert( !format2hasFixedSize( resolutions.at( i ).second ) ||
                computeAcquisitionSize( resolutions.at( i ) ) == measures.at( i ).m_size );
        assert( measures.at( i ).getResolution() == resolutions.at( i ) );
        assert( !measures.at( i ).getResolution().first.empty() );
        assert( measures.at( i ).getResolution().second != Format::NONE );
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
        while ( !m_input->isEnd() ) {
            acqs.emplace_back( getAcquisition() );
            ++nReadAcqs;
        }
    }
    catch ( std::exception& e ) {
        std::cout << "[InputSensor] catch exception : " << e.what() << std::endl;
        throw;
    }

    return acqs;
}

io::Input& InputSensor::getInput() const {
    return *m_input;
}

} // namespace hub
