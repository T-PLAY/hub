
#pragma once

#include <mutex>

#include "Client.hpp"

#include <Net/ClientSocket.hpp>

class StreamerClient;

class ViewerClient : public Client
{
  public:
    ViewerClient( Server& server, int iClient, hub::net::ClientSocket&& sock );
    ~ViewerClient();

    std::string headerMsg() const override;

//    void notifyNewStreamer(const StreamerClient& streamer , const std::string &syncStream) const;
    void notifyNewStreamer(const StreamerClient& streamer) const;
    void notifyDelStreamer(const StreamerClient& streamer
                            ) const;

  private:
    std::thread m_thread;

    hub::net::ClientSocket m_socket;
    mutable std::mutex m_mtxSocket;
};
