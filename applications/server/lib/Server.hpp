#pragma once

#include <deque>
#include <functional>
#include <map>
#include <mutex>
//#include <semaphore.h>
//#include <semaphore>

#include <Acquisition.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>

#include "ServerSocket.hpp"

class Server;

// class Streamer : public io::OutputInterface, public net::ClientSocket
//{
//   public:
//     Streamer( net::ClientSocket&& clientSocket ); // server side
// };

/////////////////////////// STREAM_VIEWER ///////////////////////////////

// class StreamViewer : public io::InputInterface, public net::ClientSocket
//{
//   public:
//     StreamViewer( net::ClientSocket&& clientSocket ); // server side
// };

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

class StreamViewer;

class Streamer : public Client
{
  public:
    Streamer( Server& server, int iClient, hub::net::ClientSocket&& sock );
    ~Streamer();

    std::string headerMsg() const override;

    const hub::InputSensor& getInputSensor() const;

    void addStreamViewer( StreamViewer* streamViewer );
    void delStreamViewer( StreamViewer* streamViewer );

    void newAcquisition (const std::string & streamerName, const hub::Acquisition & acq);

    const std::string& getStreamName() const;

    const std::map<std::string, std::list<StreamViewer*>>& getSyncViewers() const;

  private:
    std::mutex m_mtx;
    std::unique_ptr<hub::InputSensor> m_inputSensor;
    std::string m_streamName;

    std::map<std::string, std::list<StreamViewer*>> m_syncViewers;
    std::map<std::string, std::deque<hub::Acquisition>> m_syncAcqs;

    //    friend class Server;
};

class Viewer : public Client
{
  public:
    Viewer( Server& server, int iClient, hub::net::ClientSocket&& sock );

    std::string headerMsg() const override;

    void notifyNewStreamer( const Streamer& streamer ) const;
    void notifyDelStreamer( const std::string & streamerName ) const;

  private:
    hub::net::ClientSocket m_socket;

    //    friend class Server;
};

class StreamViewer : public Client
{
  public:
    StreamViewer( Server& server, int iClient, hub::net::ClientSocket&& sock );
    ~StreamViewer();

    std::string headerMsg() const override;

    void update( const hub::Acquisition& acq );

    const std::string& getSyncStreamName() const;
    const std::string& getStreamName() const;

  private:
    std::unique_ptr<hub::OutputSensor> m_outputSensor;
    std::mutex m_mtxOutputSensor;
    std::string m_streamName;

    std::string m_syncStreamName;

    std::thread m_thread;
    bool m_isKilled     = false;
    bool m_updateFailed = false;

    //    friend class Server;
};

class Server
{
  public:
    Server();
    Server( int port );
    ~Server();

    std::string headerMsg() const;

    void run();
    void asyncRun();
    void stop();

    Client* initClient( hub::net::ClientSocket&& sock, int iClient );
    std::string getStatus();

    void addStreamer( Streamer* streamer );
    void addStreamViewer( StreamViewer* streamViewer );
    void addViewer( Viewer* viewer );

    void delStreamer( Streamer* streamer );
    void delStreamViewer( StreamViewer* streamViewer );
    void delViewer( Viewer* viewer );

    void newAcquisition( Streamer* streamer, hub::Acquisition acq );

  private:
    std::map<std::string, Streamer*> m_streamers;
//    mutable std::mutex m_mtxStreamers;
    std::list<Viewer*> m_viewers;
    std::map<std::string, std::list<StreamViewer*>> m_streamViewers;

//    std::mutex m_mtx;

  private:
    hub::net::ServerSocket mServerSock;
    std::list<Client*> m_clients;

    std::thread m_thread;
    int m_maxClients = 100;

  public:
    void setMaxClients( int maxThreads );
    const std::map<std::string, Streamer*>& getStreamers() const;
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
//     const hub::net::ClientSocket* const mSock = nullptr;

//    void notifyNewStreamer( const Streamer& streamer ) const;
//};
