
#pragma once

#include "Client.hpp"

//#include <Acquisition.hpp>
//#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
//#include <OutputSensor.hpp>

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

    //    friend class Server;
};
