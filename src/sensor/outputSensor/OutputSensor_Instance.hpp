
#pragma once

#include <sensor/OutputSensor.hpp>

namespace hub {
namespace sensor {
namespace outputSensor {

class OutputSensor_Instance
{
  public:
    OutputSensor_Instance()                               = default;
    OutputSensor_Instance( OutputSensor_Instance&& )      = delete;
    OutputSensor_Instance( const OutputSensor_Instance& ) = delete;

    ~OutputSensor_Instance() {
        if ( m_running ) { stop(); }
    }

    virtual void routine()  = 0;

    void run() {
        assert(! m_running);
        m_running = true;
        routine();
    }

    void asyncRun() {
        m_running = true;
        m_thread = std::thread( [this]() { routine(); } );
    }

    void stop() {
        assert( !m_running );
        m_running = false;
        if ( m_thread.joinable() ) { m_thread.join(); }
    }

  protected:
    std::thread m_thread;
    bool m_running = false;
};

} // namespace outputSensor
} // namespace sensor
} // namespace hub
