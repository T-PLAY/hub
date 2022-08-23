#pragma once

#include <mutex>

#include "Interface.hpp"
//#include "Net/Server.hpp"
#include "Net/ClientSocket.hpp"
#include "Net/ServerSocket.hpp"

namespace hub {
namespace io {

// class Streamer;
// class Viewer;
// class StreamViewer;

// class Client : public virtual Interface
//{
//   public:
//     enum class Type { NONE, STREAMER, VIEWER, STREAM_VIEWER, COUNT };
//     friend std::ostream& operator<<( std::ostream& os, const Type& type );

//    enum class Message {
//        NONE,
//        PING,
//        SYNC,
//        DATA,
//        OK,
//        CLOSE,
//        DEL_STREAMER,
//        NEW_STREAMER,
//        NOT_FOUND,
//        FOUND,
//        COUNT
//    };
//    friend std::ostream& operator<<( std::ostream& os, const Message& msg );

//    Client( net::ClientSocket&& clientSocket );

//    Client( Client&& client )      = default;
//    Client( const Client& client ) = delete;

//    const Client& operator=( const Client& client ) = delete;

//    //    virtual ~Client() = default;

////    virtual void startAsyncRoutine( Server* server, int iThread ) = 0;
////    static Client* init( net::ClientSocket&& sock );

//  protected:
//    void close() override;
//    void write( const unsigned char* data, size_t len ) const override;
//    void read( unsigned char* data, size_t len ) const override;

//  private:
//    std::unique_ptr<net::ClientSocket> m_socket;
//};

class Streamer : public OutputInterface, public net::ClientSocket
{
  public:
    Streamer( const std::string& sensorName,
              net::ClientSocket&& clientSocket = net::ClientSocket() );
    Streamer(
              net::ClientSocket&& clientSocket ); // server side

  protected:
    //    void startAsyncRoutine( Server* server, int iThread ) override;

  private:
};

class StreamViewer : public InputInterface, public net::ClientSocket
{
  public:
    StreamViewer( const std::string& sensorName,
                  const std::string& syncSensorName = "",
                  net::ClientSocket&& clientSocket  = net::ClientSocket() );
    StreamViewer( net::ClientSocket&& clientSocket ); // server side

  protected:
    //    void startAsyncRoutine( Server* server, int iThread ) override;

  private:
};

class Viewer : public net::ClientSocket
{
  public:
    //    Viewer( const std::string& sensorName,
    //            const std::string& syncSensorName = "",
    //            net::ClientSocket&& clientSocket  = net::ClientSocket() );
    Viewer(ClientSocket &&clientSocket, std::function<void (const std::string &, const SensorSpec &)> onNewStreamer, std::function<void (const std::string &)> onDelStreamer);
    ~Viewer();

protected:
    //    void startAsyncRoutine( Server* server, int iThread ) override;

  private:
    std::thread m_thread;
    bool m_stopThread = false;
    std::function<void(const std::string & sensorName, const SensorSpec & sensorSpec)> m_onNewStreamer;
    std::function<void(const std::string & sensorName)> m_onDelStreamer;
};

} // namespace io
} // namespace hub
