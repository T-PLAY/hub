#pragma once

#include <deque>
#include <functional>
#include <map>

//#include "Net/Socket.hpp"
#include "ClientSocket.hpp"
#include "InputSensor.hpp"
#include "OutputSensor.hpp"
#include "ServerSocket.hpp"

//#include "IO/Client.hpp"

#include <mutex>

namespace hub {
namespace net {


class Server;

class Client{
public:
    Client(Server & server, int iClient);

    std::string headerMsg();

protected:
    Server & m_server;
    int m_iClient;
};

class Streamer : public Client
{
public:
    Streamer(Server & server, int iClient, ClientSocket && sock);

    std::string headerMsg();

//private:
    std::unique_ptr<InputSensor> m_inputSensor;
    std::string m_streamName;
};

class Viewer : public Client
{
public:
    Viewer(Server & server, int iClient, ClientSocket && sock);

    std::string headerMsg();

    void notifyNewStreamer( const Streamer& streamer ) const;
};

class StreamViewer : public Client
{
public:
    StreamViewer(Server & server, int iClient, ClientSocket && sock);

    std::string headerMsg();

//    void send(const Acquisition & acq);


//private:
    std::unique_ptr<OutputSensor> m_outputSensor;
    std::string m_streamName;
};

class Server
{
  public:
    Server();
    Server( int port );
    ~Server();

    std::string headerMsg();

    void run();
    void asyncRun();
    void stop();

    Client * initClient(net::ClientSocket && sock, int iClient);
        std::string getStatus() const;

public:

    std::map<std::string, Streamer*> m_streamers;
    std::list<Viewer*> m_viewers;
//    std::list<StreamViewer*> m_streamViewers;
    std::map<std::string, std::list<StreamViewer*>> m_streamViewers;

  private:
    net::ServerSocket mServerSock;
    std::list<Client*> m_clients;

    //    std::map<std::string, Streamer*> mStreamers;
    //    std::list<Viewer*> mViewers;

    std::mutex mMtx;
    std::thread m_thread;
    int m_maxClients = 100;
    //    bool m_wantToStop = false;
  public:
    void setMaxClients( int maxThreads );
};

// struct Streamer {
//     hub::InputSensor mInputSensor;
//     std::string mSensorName;

//    std::list<hub::OutputSensor> mOutputSensors;
//    std::map<std::string, std::list<hub::OutputSensor>> mSensor2syncViewers;

//    Streamer* mSyncMaster = nullptr;
//    std::map<std::string, std::deque<hub::Acquisition>> mSensor2acqs;
//};

// struct Viewer {
//     const ClientSocket* const mSock = nullptr;

//    void notifyNewStreamer( const Streamer& streamer ) const;
//};

} // namespace net
} // namespace hub
