#pragma once

//#include "SensorSpec.hpp"
//#include "Acquisition.hpp"

//#include "core/Input.hpp"

//namespace hub {
//namespace sensor {

/////
///// \brief The Sensor class
///// describes what a sensor can be, it's defined from the constant specifities of a sensor
///// and uses a communication bus to transmit the data.
///// \note Not user instantiable class.
/////
//class SRC_API Sensor
//{
//  protected:
//    ///
//    /// \brief Sensor
//    /// \param sensorSpec
//    /// [in] user defined sensor spec
//    ///
//    explicit Sensor( const SensorSpec& sensorSpec );
//    Sensor( const Sensor& sensor ) = delete;

//    Sensor& operator=( const Sensor& sensor ) = delete;

////    Sensor(Input & input) :

//    ~Sensor();

//    Acquisition acq() const {
//        return Acquisition{m_spec.getResolutions()};
//    }

//  public:
//    ///
//    /// \brief getSpec
//    /// \return
//    ///
//    const SensorSpec& getSpec() const;

//  protected:
//    ///
//    /// \brief m_spec
//    ///
//    SensorSpec m_spec;
//};

/////////////////////////////////////////////////// INLINE
///////////////////////////////////////////////////////////////////////////

//inline const SensorSpec& Sensor::getSpec() const {
//    return m_spec;
//}

//} // namespace sensor
//} // namespace hub
