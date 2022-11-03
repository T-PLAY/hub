
#pragma once

#include <mutex>

#include "Client.hpp"

#include <Net/ClientSocket.hpp>

class StreamerClient;

class ViewerClient : public Client
{
  public:
    ViewerClient( Server& server, int iClient, hub::net::ClientSocket&& sock );

    std::string headerMsg() const override;

    void notifyNewStreamer( const StreamerClient& streamer ) const;
    void notifyDelStreamer( const std::string& streamerName,
                            const hub::SensorSpec& sensorSpec ) const;

  private:
    hub::net::ClientSocket m_socket;
    mutable std::mutex m_mtxSocket;
};
