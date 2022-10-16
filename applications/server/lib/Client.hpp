#pragma once

class Server;
#include <string>

class Client
{
  public:
    Client( Server& server, int iClient );
    virtual ~Client() = default;

    virtual std::string headerMsg() const;

  protected:
    Server& m_server;
    int m_iClient;
};
