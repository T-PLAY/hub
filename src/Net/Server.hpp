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

class Client
{
  public:
    Client( Server& server, int iClient );
    virtual ~Client() = default;

    virtual std::string headerMsg();

  protected:
    Server& m_server;
    int m_iClient;
};

class StreamViewer;

class Streamer : public Client
{
  public:
    Streamer( Server& server, int iClient, ClientSocket&& sock );
    ~Streamer();

    std::string headerMsg() override;

    const InputSensor& getInputSensor() const;

    const std::string &getStreamName() const;

private:
    std::unique_ptr<InputSensor> m_inputSensor;
    std::string m_streamName;

    std::map<std::string, std::list<StreamViewer*>> m_syncViewers;
    std::map<std::string, std::deque<hub::Acquisition>> m_syncAcqs;

    friend class Server;
};

class Viewer : public Client
{
  public:
    Viewer( Server& server, int iClient, ClientSocket&& sock );

    std::string headerMsg() override;

    void notifyNewStreamer( const Streamer& streamer ) const;
};

class StreamViewer : public Client
{
  public:
    StreamViewer( Server& server, int iClient, ClientSocket&& sock );
    ~StreamViewer();

    std::string headerMsg() override;

//    void send( const Acquisition& acq );
//    void send(Acquisition && acq);
    void update(const Acquisition &acq);

  private:
    std::unique_ptr<OutputSensor> m_outputSensor;
    std::string m_streamName;

//    std::deque<Acquisition> m_acquisitions;
//    std::mutex m_mtxAcquisitions;
    std::string m_syncStreamName;
//    std::deque<Acquisition> m_syncAcquisitions;

    friend class Server;
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

    Client* initClient( net::ClientSocket&& sock, int iClient );
    std::string getStatus();

    void addStreamer(Streamer * streamer);
    void addStreamViewer(StreamViewer * streamViewer);

    void delStreamer(Streamer * streamer);
    void delStreamViewer(StreamViewer * streamViewer);

    void newAcquisition(Streamer * streamer, Acquisition acq);

  private:
    std::map<std::string, Streamer*> m_streamers;
//    std::mutex m_mtxStreamers;
    std::list<Viewer*> m_viewers;
//    std::mutex m_mtxViewers;
    //    std::list<StreamViewer*> m_streamViewers;
    std::map<std::string, std::list<StreamViewer*>> m_streamViewers;
//    std::mutex m_mtxStreamViewers;

    std::mutex m_mtx;

  private:
    net::ServerSocket mServerSock;
    std::list<Client*> m_clients;

    //    std::map<std::string, Streamer*> mStreamers;
    //    std::list<Viewer*> mViewers;

    std::thread m_thread;
    int m_maxClients = 100;
    //    bool m_wantToStop = false;
  public:
    void setMaxClients( int maxThreads );
    const std::map<std::string, Streamer*>& getStreamers() const;
//    std::list<StreamViewer *> &getStreamViewers(const std::string & streamName);
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
