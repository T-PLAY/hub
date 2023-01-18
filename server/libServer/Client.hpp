#pragma once

#include <mutex>
#include <string>

class Server;

class Client
{
  public:
    Client( Server& server, int iClient );
    virtual ~Client() = default;

    virtual std::string headerMsg() const;
    void printStatusMessage( const std::string& message ) const;

  protected:
    Server& m_server;
    int m_iClient;

    static std::mutex s_mtxCout;
};
