
#pragma once

#include <sensor/OutputSensor.hpp>

namespace hub {
namespace sensor {
namespace outputSensor {

class OutputSensor_Instance
{
  public:
    // OutputSensor_Instance()                               = default;
    OutputSensor_Instance( OutputSensor_Instance&& )      = delete;
    OutputSensor_Instance( const OutputSensor_Instance& ) = delete;
    OutputSensor_Instance( int port = HUB_SERVICE_PORT, std::string ipv4 = HUB_SERVICE_IPV4 ) :
        m_port { port }, m_ipv4 { ipv4 } {};

    virtual void init() const = 0;

    virtual ~OutputSensor_Instance() {
        if ( m_running ) { stop(); }
    }

    void run() {
        if ( !m_inited ) init();

        assert( !m_running );
        m_running = true;
        routine();
    }

    void asyncRun() {
        if ( !m_inited ) init();

        m_running = true;
        m_thread  = std::thread( [this]() { routine(); } );
    }

    void stop() {
        assert( m_running );
        m_running = false;
        if ( m_thread.joinable() ) { m_thread.join(); }
    }
    const std::string& getName() const {
        if ( !m_inited ) init();
        return m_sensorSpec.getSensorName();
    }
    const hub::sensor::SensorSpec& getSensorSpec() const {
        if ( !m_inited ) init();
        return m_sensorSpec;
    }

    // protected:
    virtual void routine() = 0;

    std::function<void( const std::string& streamName, const hub::sensor::Acquisition& acq )>
        onNewAcq;

  protected:
    std::thread m_thread;
    bool m_running = false;
    int m_port;
    std::string m_ipv4;
    mutable hub::sensor::SensorSpec m_sensorSpec;
    mutable std::string m_streamName;
    mutable bool m_inited = false;
};

} // namespace outputSensor
} // namespace sensor
} // namespace hub
