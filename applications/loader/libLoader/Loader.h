
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
    std::string m_sensorName;

    bool operator<( const Snap& other ) const { return m_acq.m_start < other.m_acq.m_start; }
};

class Loader : public QObject
{
    Q_OBJECT
  public:
    Loader( const std::string& ipv4 = hub::net::s_defaultServiceIp,
            const int& port         = hub::net::s_defaultServicePort );

    ~Loader();

//    void load( const std::string& path );
    void load( const std::set<std::string> & paths);
    void unload();

    void play();
    void stop();

  signals:
    void pathLoaded();
    void acqChanged( int nAcq );
    void playEnded();

  public slots:

  private:
    std::vector<Snap> m_snaps;
    std::set<std::string> m_loadedPaths;

    const std::string& m_ipv4;
    const int& m_port;

    std::thread* m_thread = nullptr;
    bool m_isPlaying      = false;
    bool m_autoPlay       = true;
    bool m_autoLoop       = true;

    int m_iAcq;

    std::map<std::string, std::unique_ptr<hub::OutputSensor>> m_outputStreams;

  public:
    bool isPlaying() const;
    bool isLoaded() const;
    void setAutoLoop( bool newAutoLoop );
    void setAutoPlay( bool newAutoPlay );
    int getNAcq() const;
    void setIAcq( int newIAcq );
};
