
#pragma once

#include <mutex>
#include <thread>

#include <Net/ClientSocket.hpp>

#include "Client.hpp"

class StreamerClient;

class ViewerClient : public Client
{
  public:
    ViewerClient( Server& server, int iClient, hub::net::ClientSocket&& sock );
    ~ViewerClient();

    std::string headerMsg() const override;

    //    void notifyNewStreamer(const StreamerClient& streamer , const std::string &syncStream)
    //    const;
    void notifyNewStreamer( const StreamerClient& streamer );
    void notifyDelStreamer( const StreamerClient& streamer );

  private:
    std::thread m_thread;

    hub::net::ClientSocket m_socket;
    std::mutex m_mtxSocket;
};
