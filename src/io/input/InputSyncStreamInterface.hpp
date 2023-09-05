#pragma once

#include "Input.hpp"
#include "InputStream.hpp"


namespace hub {


namespace input {

///
/// \brief The InputSyncStreamInterface class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
///
template <class InputStream = InputStream>
//template <class InputStream>
class SRC_API InputSyncStreamInterface : public Input
{
  public:
    ///
    /// \brief InputSyncStreamInterface
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
    InputSyncStreamInterface( const std::string& streamName,
                     const std::string& streamName2,
                     const std::string& ipv4 = InputStream::s_defaultIpv4,
//                     int port = io::s_serverDefaultPort
//                     int port = io::StreamInterface::getDefaultPort()
//                     int port = InputStream::getDefaultPort()
                     int port = InputStream::s_defaultPort
                     );

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
InputSyncStreamInterface<InputStream>::InputSyncStreamInterface( const std::string& streamName,
                                  const std::string& streamName2,
                                  const std::string& ipv4,
                                  int port ) :
    m_inputStream( streamName, ipv4, port ),
    m_inputStream2( streamName2, ipv4, port )
{}

template <class InputStream>
inline bool InputSyncStreamInterface<InputStream>::isOpen() const {
    return m_inputStream.isOpen() || m_inputStream2.isOpen();
}

template <class InputStream>
inline void InputSyncStreamInterface<InputStream>::read( unsigned char* data, size_t len ) {
    assert( false );
}

template <class InputStream>
inline void InputSyncStreamInterface<InputStream>::close() {
    std::cout << "[InputSyncStreamInterface] close()" << std::endl;
    if ( m_inputStream.isOpen() ) m_inputStream.close();
    if ( m_inputStream2.isOpen() ) m_inputStream2.close();

    assert( !m_inputStream.isOpen() && !m_inputStream2.isOpen() );
}

template <class InputStream>
inline bool InputSyncStreamInterface<InputStream>::isEnd() const {
    return m_inputStream.isEnd() || m_inputStream2.isEnd();
}


template <class InputStream>
inline void InputSyncStreamInterface<InputStream>::read( Acquisition& acq ) {

    Input& input = m_inputStream;
    Input& input2       = m_inputStream2;

    input >> input2 >> acq;
//    (void)(m_inputStream >> m_inputStream2 >> acq);
}

template <class InputStream>
inline void InputSyncStreamInterface<InputStream>::read( SensorSpec& sensorSpec ) {
    m_inputStream.read( sensorSpec );
    SensorSpec sensorSpec2;
    m_inputStream2.read( sensorSpec2 );

    sensorSpec = sensorSpec + sensorSpec2;
}

} // namespace input
} // namespace hub
