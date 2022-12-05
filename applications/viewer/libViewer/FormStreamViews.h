#pragma once

#include <map>

#include <QComboBox>
#include <QMdiArea>
#include <QStringListModel>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

#include <Viewer.hpp>

#include <FormStreamView.h>
#include <FormServerView.h>
//#include <MainWindowStreamView.h>

// class FormStreamViews;

// class ViewerQt : public QObject
//{
//     Q_OBJECT
//   public:
//     ViewerQt( FormStreamViews & streamViews, const std::string& ipv4, const int& port );
//     ~ViewerQt();

//  signals:
//    void serverConnected();
//    void serverDisconnected();
//    void newStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
//    void delStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
////    void newAcquisition( const std::string& streamName, hub::Acquisition&& acq );

//  public:
//    hub::Viewer* m_viewer = nullptr;
//    FormStreamViews & m_streamViews;
//};

namespace Ui {
class FormStreamViews;
}

class FormStreamViews : public QWidget
{
    Q_OBJECT

  public:
    static bool s_autoConnect;
    static bool s_autoSync;
    static const std::string * s_ipv4;
    static const int * s_port;

    explicit FormStreamViews( QWidget* parent = nullptr );
    ~FormStreamViews();

    std::vector<std::pair<std::string, std::string>>  getActiveStreams();
    void initViewer(std::function<bool (const std::string &streamName, const hub::SensorSpec &)> newStreamerAdded,
//        std::function<bool( const std::string&, const hub::SensorSpec& )> onNewStreamer= {},
//        std::function<void( const std::string&, const hub::SensorSpec& )> onDelStreamer= {},
//        std::function<void( const std::string&, int )> onServerConnected       = {}            ,
//        std::function<void( const std::string&, int )> onServerDisconnected = {}               ,
            std::function<void( const std::string& streamName, const hub::Acquisition& )> onNewAcquisition
            );

  signals:
    void newStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void delStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void serverConnected( const std::string& ipv4, int port );
    void serverDisconnected( const std::string& ipv4, int port );
//    void newAcquisition( const std::string& streamName, const hub::Acquisition& acq );

    #ifndef USE_COMPLETE_VIEWER
        void startStream( const std::string& streamName, const hub::SensorSpec& sensorSpec, const std::string& syncSensorName );
        void stopStream( const std::string& streamName, const hub::SensorSpec& sensorSpec, const std::string& syncSensorName );
    //    void streamingStarted( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    //    void streamingStopped( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    #endif

  private slots:
    bool onNewStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void onDelStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void onServerConnected( const std::string& ipv4, int port );
    void onServerDisconnected( const std::string& ipv4, int port );

    void onStartStream( const std::string& streamName, const hub::SensorSpec& sensorSpec, const std::string& syncSensorName );
    void onStopStream( const std::string& streamName, const hub::SensorSpec& sensorSpec, const std::string& syncSensorName);
//    void onNewAcquisition( const std::string& streamName, const hub::Acquisition& acq );

  private:
    Ui::FormStreamViews* ui;

    //    ViewerQt* m_viewerQt = nullptr;
//    hub::Viewer* m_viewer = nullptr;

//    std::string m_ipv4;
//    int m_port;
    FormServerView * m_serverView = nullptr;

    std::map<std::string, FormStreamView*> m_streamViews;

//    bool m_serverConnected = false;
//    bool m_serverPing      = false;

    QStringListModel m_sensorModel;


  public:
    //    const FormStreamView& getStreamView( const std::string& streamName ) const;

    const std::string& getIpv4() const;
    const int& getPort() const;

  private slots:
//    void on_lineEdit_ip_textChanged( const QString& ipv4 );
//    void on_spinBox_port_valueChanged( int port );
    void on_checkBox_sync_toggled(bool checked);
    void on_checkBox_connect_toggled(bool checked);
};
