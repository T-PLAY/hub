#pragma once

#include <string>
#include <mutex>

class Server;

class Client
{
  public:
    Client( Server& server, int iClient );
    virtual ~Client() = default;

    virtual std::string headerMsg() const;
    void printStatusMessage(const std::string & message) const;

  protected:
    Server& m_server;
    int m_iClient;

    static std::mutex s_mtxCout;
};
