
#pragma once

#include <future>
#include <string>

#include <QItemSelection>
#include <QListView>
#include <QObject>
#include <QStringListModel>
#include <QTreeView>

#include <Frame.h>
#include <IO/Ram.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <set>

#include <Configurations.hpp>

struct Snap {
    hub::Acquisition m_acq;
//    int iStream;
    std::string m_sensorName;

    bool operator<(const Snap& other) const
    {
        return m_acq.m_start < other.m_acq.m_start;
    }
};

class Loader : public QObject
{
    Q_OBJECT
  public:
//    Loader( const std::string& outputPostFixName = "", bool autoPlay = true );
    Loader( const std::string & ipv4 = hub::net::s_defaultServiceIp, const int & port = hub::net::s_defaultServicePort );

    ~Loader();

    void load( const std::string& path );
//    void update();
    void unload();

    void play();
    void stop();

  signals:
    void pathLoaded();
    void acqChanged(int nAcq);
//    void loopEnded();
    void playEnded();

  public slots:

  private:
//    std::string m_outputPostfixName = "";

//    std::vector<std::unique_ptr<hub::InputSensor>> m_inputStreams;

//    std::map<std::string, std::vector<hub::Acquisition>> m_sensorName2acquisitions;
//    std::set<Snap> m_snaps;
    std::vector<Snap> m_snaps;
    std::string m_loadedPath = "";

    const std::string & m_ipv4;
    const int & m_port;

    std::thread* m_thread = nullptr;
    bool m_isPlaying      = false;
    bool m_autoPlay = true;
    bool m_autoLoop = true;

    int m_iAcq;

    std::map<std::string, std::unique_ptr<hub::OutputSensor>> m_outputStreams;
//    std::vector<std::unique_ptr<hub::OutputSensor>> m_outputStreams;

  public:
    bool isPlaying() const;
    bool isLoaded() const;
//    const std::string& getLoadedPath() const;
    //    const std::vector<hub::Acquisition>& getAcquisitions( const std::string& sensorName ) const;
    void setAutoLoop(bool newAutoLoop);
    void setAutoPlay(bool newAutoPlay);
    int getNAcq() const;
    void setIAcq(int newIAcq);
};
