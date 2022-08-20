#pragma once

#include <mutex>

#include "Interface.hpp"
//#include "Net/Server.hpp"
#include "Net/ServerSocket.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {
namespace io {

class Streamer;
class Viewer;
class StreamViewer;

class Server
{
  public:
    Server(  );
    Server( int port );

    void run();

//    std::string getStatus() const;

  private:
    net::ServerSocket mServerSock;

    std::map<std::string, Streamer*> m_streamers;
    std::list<Viewer*> m_viewers;
    std::list<StreamViewer*> m_streamViewers;

//    std::map<std::string, Streamer*> mStreamers;
//    std::list<Viewer*> mViewers;

    std::mutex mMtx;
};

class Client : public Interface
{
  public:
    enum class Type { NONE, STREAMER, VIEWER, STREAM_VIEWER, COUNT };
    friend std::ostream& operator<<( std::ostream& os, const Type& type );

    enum class Message {
        NONE,
        PING,
        SYNC,
        DATA,
        OK,
        CLOSE,
        DEL_STREAMER,
        NEW_STREAMER,
        NOT_FOUND,
        FOUND,
        COUNT
    };
    friend std::ostream& operator<<( std::ostream& os, const Message& msg );

    Client( net::ClientSocket&& clientSocket );
    Client( const Client& client ) = delete;

    //    virtual ~Client() = default;

    virtual void startAsyncRoutine( Server* server, int iThread ) = 0;
    static Client* init(net::ClientSocket &&sock );

  protected:
    void close() override;
    void write( const unsigned char* data, size_t len ) const override;
    void read( unsigned char* data, size_t len ) const override;

  private:
    std::unique_ptr<net::ClientSocket> m_socket;
};

class Streamer : public Client
{
  public:
    Streamer( net::ClientSocket&& clientSocket );
  protected:
    void startAsyncRoutine( Server* server, int iThread ) override;

  private:
};

class Viewer : public Client
{
  public:
    Viewer( net::ClientSocket&& clientSocket );
  protected:
    void startAsyncRoutine( Server* server, int iThread ) override;

  private:
};

class StreamViewer : public Client
{
  public:
    StreamViewer( net::ClientSocket&& clientSocket );
  protected:
    void startAsyncRoutine(Server *server, int iThread ) override;

  private:
};



} // namespace io
} // namespace hub
