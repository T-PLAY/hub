
#pragma once

#include "Input.hpp"
#include "InputStream.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The InputSyncStream class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
///
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
    /// \param clientSocket
    /// [in] is an existing connection to a hub server.
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
    /// \return
    ///
    void read( Acquisition& acq ) override;

    void read( SensorSpec& sensorSpec ) override;

  private:
    InputStream m_inputStream;
    InputStream m_inputStream2;
    std::list<Acquisition> m_lastAcqs;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputSyncStream::isOpen() const {
    return m_inputStream.isOpen() || m_inputStream2.isOpen();
}

inline void InputSyncStream::read( unsigned char* data, size_t len ) {
    assert( false );
}

inline void InputSyncStream::close() {
    std::cout << "[InputSyncStream] close()" << std::endl;
    if ( m_inputStream.isOpen() ) m_inputStream.close();
    if ( m_inputStream2.isOpen() ) m_inputStream2.close();

    assert( !m_inputStream.isOpen() && !m_inputStream2.isOpen() );
}

inline bool InputSyncStream::isEnd() const {
    return m_inputStream.isEnd() || m_inputStream2.isEnd();
}

} // namespace io
} // namespace hub
