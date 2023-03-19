#pragma once

#include <mutex>
#include <string>


namespace hub {
namespace server {

class Server;

///
/// \brief The Client class
///
class Client
{
//  public:
//  protected:
private:
    Client( Server* server, int iClient );
    virtual ~Client();

    virtual std::string headerMsg() const;
    void printStatusMessage( const std::string& message ) const;

    void setServer( Server* newServer );

    virtual void end() = 0;

//  protected:
  private:
    Server* m_server = nullptr;
    int m_iClient;

//    static std::mutex s_mtxCout;

    friend class Server;
    friend class StreamerClient;
    friend class StreamViewerClient;
    friend class ViewerClient;
    friend class AskerClient;
};


} // server
} // hub
