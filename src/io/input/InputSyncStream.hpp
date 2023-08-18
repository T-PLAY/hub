
#pragma once

#include "Input.hpp"
#include "InputStream.hpp"

//#include "InputStreamMqtt.hpp"
//#include "InputStreamServer.hpp"
//#include "net/ClientSocket.hpp"

namespace hub {

using namespace io;

namespace input {

///
/// \brief The InputSyncStream class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
///
template <class InputStream = InputStream>
//template <class InputStream>
class SRC_API InputSyncStream : public Input
{
  public:
    ///
    /// \brief InputSyncStream
    /// is used to instantiate an InputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream.
    /// \param syncStreamName
    /// [in] is an unique identifier name of stream.
    /// \param ipv4
    /// \param port
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to link (by their names) needs to be active in the server hub.
    /// \exception hub::net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is not connected to the server.
    ///
    InputSyncStream( const std::string& streamName,
                     const std::string& syncStreamName,
                     const std::string& ipv4 = net::s_defaultServiceIp,
                     int port                = net::s_defaultServicePort );

  protected:
  public:
    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    void read( unsigned char* data, size_t len ) override;

    ///
    /// \brief close
    ///
    void close() override;

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override;

    ///
    /// \brief getAcq
    /// \param acq
    ///
    void read( Acquisition& acq ) override;

    ///
    /// \brief read
    /// \param sensorSpec
    ///
    void read( SensorSpec& sensorSpec ) override;



  private:
    InputStream m_inputStream;
    InputStream m_inputStream2;
    std::list<Acquisition> m_lastAcqs;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class InputStream>
InputSyncStream<InputStream>::InputSyncStream( const std::string& streamName,
                                  const std::string& syncStreamName,
                                  const std::string& ipv4,
                                  int port ) :
    m_inputStream( streamName, ipv4, port ),
    m_inputStream2( syncStreamName, ipv4, port )
{}

template <class InputStream>
inline bool InputSyncStream<InputStream>::isOpen() const {
    return m_inputStream.isOpen() || m_inputStream2.isOpen();
}

template <class InputStream>
inline void InputSyncStream<InputStream>::read( unsigned char* data, size_t len ) {
    assert( false );
}

template <class InputStream>
inline void InputSyncStream<InputStream>::close() {
    std::cout << "[InputSyncStream] close()" << std::endl;
    if ( m_inputStream.isOpen() ) m_inputStream.close();
    if ( m_inputStream2.isOpen() ) m_inputStream2.close();

    assert( !m_inputStream.isOpen() && !m_inputStream2.isOpen() );
}

template <class InputStream>
inline bool InputSyncStream<InputStream>::isEnd() const {
    return m_inputStream.isEnd() || m_inputStream2.isEnd();
}


template <class InputStream>
inline void InputSyncStream<InputStream>::read( Acquisition& acq ) {

    Input& masterInput = m_inputStream;
    Input& input       = m_inputStream2;

    masterInput >> input >> acq;
}

template <class InputStream>
inline void InputSyncStream<InputStream>::read( SensorSpec& sensorSpec ) {
    m_inputStream.read( sensorSpec );
    SensorSpec sensorSpec2;
    m_inputStream2.read( sensorSpec2 );

    sensorSpec = sensorSpec + sensorSpec2;
}

} // namespace input
} // namespace hub
