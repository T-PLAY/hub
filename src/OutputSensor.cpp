#include "OutputSensor.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>

namespace hub {

void OutputSensor::operator<<( const Acquisition& acquisition ) const {
//    if (acquisition.m_start == -1 && acquisition.m_end == -1) {
//        m_interface.write(acquisition);
//        return;
//    }
    assert( acquisition.getSize() == m_spec.getAcquisitionSize() );

    assert( acquisition.m_start <= acquisition.m_end );

    const auto& resolutions = m_spec.getResolutions();
    const auto& measures    = acquisition.getMeasures();
    assert( resolutions.size() == measures.size() );
    for ( int i = 0; i < resolutions.size(); ++i ) {
        assert( computeAcquisitionSize( resolutions.at( i ) ) ==
                measures.at( i ).m_size );
    }

    m_interface.write( acquisition );
}

} // namespace hub
