#include "OutputSensor.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>

namespace hub {
namespace sensor {

OutputSensor::OutputSensor( OutputSensor&& outputSensor ) :
    Sensor( outputSensor.m_spec ),
    m_output( std::move( outputSensor.m_output ) )
//    m_output(outputSensor.m_output)
{
    outputSensor.m_moved = true;
}

OutputSensor::~OutputSensor() {
    if ( !m_moved ) {
        if ( m_output->isOpen() ) m_output->close();
        assert( !m_output->isOpen() );
    }
}

void OutputSensor::operator<<( const Acquisition& acquisition ) const {
#ifdef DEBUG
    if (! m_output->isOpen())
        return;
    assert( m_output->isOpen() ); // todo fix
    assert( !acquisition.hasFixedSize() || acquisition.getSize() == m_spec.getAcquisitionSize() );
    assert( acquisition.getStart() <= acquisition.getEnd() );

    const auto& resolutions = m_spec.getResolutions();
    const auto& measures    = acquisition.getMeasures();
    assert( resolutions.size() == measures.size() );
    for ( size_t i = 0; i < resolutions.size(); ++i ) {
        const auto& resolution = resolutions.at( i );
        const auto& format     = resolution.second;
        assert( !resolution::format2hasFixedSize( format ) ||
                resolution::computeAcquisitionSize( resolutions.at( i ) ) == measures.at( i ).getSize() );
    }
#endif

    m_output->write( acquisition );
}

Output& OutputSensor::getOutput() const {
    return *m_output;
}

} // namespace sensor
} // namespace hub
