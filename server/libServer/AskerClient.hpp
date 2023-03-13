
#pragma once

#include <mutex>
#include <thread>

#include <net/ClientSocket.hpp>

#include "Client.hpp"

class AskerClient : public Client
{
  public:
    AskerClient( Server& server, int iClient, hub::net::ClientSocket&& sock );
    ~AskerClient();

    std::string headerMsg() const override;

  private:
    std::thread m_thread;
    hub::net::ClientSocket m_socket;
};
