#include "OutputSensor.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>

namespace hub {

void OutputSensor::operator<<( const Acquisition& acquisition ) const {
    assert( !acquisition.hasFixedSize() || acquisition.getSize() == m_spec.getAcquisitionSize() );

    assert( acquisition.m_start <= acquisition.m_end );

    const auto& resolutions = m_spec.getResolutions();
    const auto& measures    = acquisition.getMeasures();
    assert( resolutions.size() == measures.size() );
    for ( int i = 0; i < resolutions.size(); ++i ) {
        const auto& resolution = resolutions.at( i );
        const auto& format     = resolution.second;
        assert( !format2hasFixedSize( format ) ||
                computeAcquisitionSize( resolutions.at( i ) ) == measures.at( i ).m_size );
    }

    m_output->write( acquisition );
}

io::Output& OutputSensor::getOutput() const {
    return *m_output;
}

} // namespace hub
