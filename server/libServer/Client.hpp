#pragma once

#include <mutex>
#include <string>

class Server;

class Client
{
  public:
    Client( Server* server, int iClient );
    virtual ~Client();

    virtual std::string headerMsg() const;
    void printStatusMessage( const std::string& message ) const;

    void setServer( Server* newServer );

  protected:
    Server* m_server = nullptr;
    int m_iClient;

    static std::mutex s_mtxCout;
};
