#include "InputSensor.hpp"

namespace hub {
namespace sensor {

InputSensor::InputSensor( InputSensor&& inputSensor ) :
    Sensor( std::move( inputSensor.m_spec ) ), m_input( std::move( inputSensor.m_input ) ) {
    inputSensor.m_moved = true;
}

InputSensor::~InputSensor() {
    if ( !m_moved ) {
        if ( m_input->isOpen() ) m_input->close();
        assert( !m_input->isOpen() );
    }
}

Acquisition InputSensor::operator>>( InputSensor& inputSensor ) {

    Input& lefInput = *m_input;
//    /* cppcheck-suppress constVariableReference */ // 2.11
    /* cppcheck-suppress constVariable */ // 2.10
    Input& rightInput       = inputSensor.getInput();
    return lefInput >> rightInput;
}

void InputSensor::operator>>( Acquisition& acq ) {

    assert( m_input->isOpen() );

    *m_input >> acq;

#ifdef DEBUG
    assert( !acq.hasFixedSize() || m_spec.getAcquisitionSize() == acq.getSize() );

    const auto& resolutions = m_spec.getResolutions();
    assert( !acq.hasFixedSize() || acq.getSize() == m_spec.getAcquisitionSize() );
    const auto& acqMeasures = acq.getMeasures();
    assert( resolutions.size() == acqMeasures.size() );
    assert( resolutions.size() > 0 );
    for ( size_t i = 0; i < resolutions.size(); ++i ) {
        const auto& resolution    = resolutions.at( i );
        const auto& acqMeasure    = acqMeasures.at( i );
        const auto& acqResolution = acqMeasure.getResolution();

        assert( !resolution::format2hasFixedSize( resolution.second ) ||
                resolution::computeAcquisitionSize( resolution ) == acqMeasure.getSize() );
        assert( !acqResolution.first.empty() );
        assert( acqResolution.second != Format::NONE );
        assert( acqResolution == resolution );
    }
#endif
}

std::vector<Acquisition> InputSensor::getAllAcquisitions() {
    std::vector<Acquisition> acqs;
    m_input->readAll( acqs );
    return acqs;
}

Input& InputSensor::getInput() const {
    return *m_input;
}

void InputSensor::close() {
    if ( m_input->isOpen() ) m_input->close();
}

} // namespace sensor
} // namespace hub
