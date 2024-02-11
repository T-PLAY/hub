/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27

#pragma once

#include <atomic>
#include <mutex>
#include <thread>

#include "core/Output.hpp"
#include "io/StreamServer2.hpp"
#include "net/ServerSocket.hpp"

namespace hub {
namespace output {

///
/// \brief The OutputStreamServer2 class
/// Describes an output communication to the server.
///
class SRC_API OutputStreamServer2 : public Output, public io::StreamServer2
{
  public:
    using Output::write;

    ///
    /// \brief OutputStreamServer2
    /// is used to instantiate an OutputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream
    /// \param clientSocket
    /// [in] is an existing connection to a hub server
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to start linking (by it name) needs to not exist in the server hub.
    /// Streaming with most of one sender is not allowed.
    /// \exception net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is already started in the server.
    ///
    OutputStreamServer2( const io::Header& header, int streamPort );

    OutputStreamServer2( const hub::io::Header& header,
                         const std::string& streamName,
                         int port                = HUB_SERVICE_PORT,
                         const std::string& ipv4 = HUB_SERVICE_IPV4 );
    ///
    /// \brief OutputStreamServer2
    /// \param outputStream
    ///
    OutputStreamServer2( OutputStreamServer2&& outputStream ) noexcept;

    ~OutputStreamServer2();

    void write( const Data_t* data, Size_t size ) override;
    void close() override;
    bool isOpen() const override;
    void setRetain( bool retain ) override;

    int getNStreamViewer() const;
    const io::Header& getHeader() const;

  private:
    void startStreaming();
    void tryConnectToServer();
    void stopStreaming();

    struct SharedData {
        SharedData( std::unique_ptr<hub::io::InputOutputSocket>&& serverSocket ) :
            m_serverSocket { std::move( serverSocket ) } {};
        SharedData() = default;

        std::unique_ptr<net::ServerSocket> m_streamSocket;
        io::Header m_header;
        std::unique_ptr<hub::io::InputOutputSocket> m_serverSocket;
        int m_streamPort = 0;
        std::list<hub::io::InputOutputSocket> m_streamViewerSocks;
        std::mutex m_mtxClientSockets;
        std::vector<hub::Data_t> m_retainedData;
        bool m_killed                          = false;
        std::atomic<bool> m_isStreaming        = false;
        std::atomic<bool> m_serverConnected    = false;
        std::atomic<bool> m_streamViewerInited = false;
        std::atomic<bool> m_streamerInited     = false;
        std::function<void( const Data_t*, Size_t )> m_writingFun;
        std::unique_ptr<std::thread> m_streamThread;
        std::unique_ptr<std::thread> m_serverThread;
        bool m_shutdown       = false;
        long long m_byteWrote = 0;
        size_t m_acqWrote = 0;
        std::chrono::high_resolution_clock::time_point m_lastClock =
            std::chrono::high_resolution_clock::now();
    };
    std::unique_ptr<SharedData> m_data;
    bool m_moved = false;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void OutputStreamServer2::close() {
    assert( false );
    m_data->m_serverSocket->close();
}

inline bool OutputStreamServer2::isOpen() const {
    return m_data->m_streamSocket->isConnected();
}

} // namespace output
} // namespace hub
