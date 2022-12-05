#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <FormWidgetLoader.h>
#include <FormServerView.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

  signals:
    void newStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void delStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void serverConnected( const std::string& ipv4, int port );
    void serverDisconnected( const std::string& ipv4, int port );
//    void newAcquisition( const std::string& streamName, const hub::Acquisition& acq );

  private slots:
    bool onNewStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void onDelStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void onServerConnected( const std::string& ipv4, int port );
    void onServerDisconnected( const std::string& ipv4, int port );
    void onNewAcquisition( const std::string& streamName, const hub::Acquisition& acq );


  private:
    Ui::MainWindow* ui;

    FormWidgetLoader * m_widgetLoader = nullptr;
    FormServerView * m_serverView = nullptr;
};

#endif // MAINWINDOW_H
