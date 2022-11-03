#include "OutputSensor.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>

namespace hub {

void OutputSensor::operator<<( const Acquisition& acquisition ) const {
    assert( acquisition.getSize() == m_spec.m_acquisitionSize );

    assert( acquisition.m_start <= acquisition.m_end );

    const auto& resolutions = m_spec.m_resolutions;
    const auto& measures    = acquisition.getMeasures();
    assert( resolutions.size() == measures.size() );
    for ( int i = 0; i < resolutions.size(); ++i ) {
        assert( SensorSpec::computeAcquisitionSize( resolutions.at( i ) ) ==
                measures.at( i ).m_size );
    }

    m_interface.write( acquisition );
}

} // namespace hub
