
#pragma once

#include "Input.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The InputSyncStream class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
///
// class SRC_API InputSyncStream : public Input, public net::ClientSocket
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
    explicit InputSyncStream( const std::string& streamName,
                          const std::string& syncStreamName,
                          net::ClientSocket&& clientSocket  = net::ClientSocket());
//                          bool mergeSyncAcqs                = true );

//#ifdef WIN32 // msvc warning C4250
//  protected:
//    void write( const unsigned char* data, size_t len ) const override;
//    void read( unsigned char* data, size_t len ) const override;
//    void close() const override;
//    bool isOpen() const override;
//    bool isEnd() const override;
//#endif

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
    /// \brief getAcquisition
    /// \return
    ///
    Acquisition getAcquisition( ) override;

    void read( SensorSpec& sensorSpec) override;

  private:
    net::ClientSocket m_clientSocket;
    net::ClientSocket m_clientSocket2;

    SensorSpec m_sensorSpec;
    SensorSpec m_sensorSpec2;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputSyncStream::isOpen() const {
    return m_clientSocket.isOpen() && m_clientSocket2.isOpen();
}

inline void InputSyncStream::read( unsigned char* data, size_t len ) {
    assert(false);
    m_clientSocket.read( data, len );
}

inline void InputSyncStream::close() {
    m_clientSocket.close();
    m_clientSocket2.close();
}

inline bool InputSyncStream::isEnd() const {
    return m_clientSocket.isEnd() || m_clientSocket2.isEnd();
}

} // namespace io
} // namespace hub
