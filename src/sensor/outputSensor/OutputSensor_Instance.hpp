
#pragma once

#include <sensor/OutputSensor.hpp>

namespace hub {
namespace sensor {
namespace outputSensor {

///
/// \brief The OutputSensor_Instance class
///
class OutputSensor_Instance
{
  public:
    // OutputSensor_Instance()                               = default;
    OutputSensor_Instance( OutputSensor_Instance&& )      = delete;
    OutputSensor_Instance( const OutputSensor_Instance& ) = delete;
    ///
    /// \brief OutputSensor_Instance
    /// \param port
    /// \param ipv4
    ///
    OutputSensor_Instance( int port = HUB_SERVICE_PORT, std::string ipv4 = HUB_SERVICE_IPV4 ) :
        m_port { port }, m_ipv4 { ipv4 } {};

    ///
    /// \brief init
    ///
    virtual void init() const = 0;

    virtual ~OutputSensor_Instance() {
        if ( m_running ) { stop(); }
    }

    ///
    /// \brief run
    ///
    void run() {
        if ( !m_inited ) init();

        assert( !m_running );
        m_running = true;
        routine();
    }

    ///
    /// \brief asyncRun
    ///
    void asyncRun() {
        if ( !m_inited ) init();

        m_running = true;
        m_thread  = std::thread( [this]() { routine(); } );
    }

    ///
    /// \brief stop
    ///
    void stop() {
        assert( m_running );
        m_running = false;
        if ( m_thread.joinable() ) { m_thread.join(); }
    }

    ///
    /// \brief getName
    /// \return
    ///
    const std::string& getName() const {
        if ( !m_inited ) init();
        return m_sensorSpec.getSensorName();
    }

    ///
    /// \brief getSensorSpec
    /// \return
    ///
    const hub::sensor::SensorSpec& getSensorSpec() const {
        if ( !m_inited ) init();
        return m_sensorSpec;
    }

    // protected:
    ///
    /// \brief routine
    ///
    virtual void routine() = 0;

    ///
    /// \brief onNewAcq
    ///
    std::function<void( const std::string& streamName, const hub::sensor::Acquisition& acq )>
        onNewAcq;

  protected:
    ///
    /// \brief m_thread
    ///
    std::thread m_thread;

    ///
    /// \brief m_running
    ///
    bool m_running = false;

    ///
    /// \brief m_port
    ///
    int m_port;

    ///
    /// \brief m_ipv4
    ///
    std::string m_ipv4;

    ///
    /// \brief m_sensorSpec
    ///
    mutable hub::sensor::SensorSpec m_sensorSpec;

    ///
    /// \brief m_streamName
    ///
    mutable std::string m_streamName;

    ///
    /// \brief m_inited
    ///
    mutable bool m_inited = false;
};

} // namespace outputSensor
} // namespace sensor
} // namespace hub
