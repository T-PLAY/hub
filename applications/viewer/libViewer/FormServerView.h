#pragma once

#include <QRegularExpression>
#include <QWidget>

#include <SensorSpec.hpp>
#include <Viewer.hpp>

namespace Ui {
class FormServerView;
}

class FormServerView : public QWidget
{
    Q_OBJECT

  public:
    explicit FormServerView(
            std::function<bool( const std::string& streamName, const hub::SensorSpec& )> onNewStreamer = {},
        std::function<void( const std::string& streamName, const hub::SensorSpec& )> onDelStreamer= {},
        std::function<void( const std::string& ipv4, int port )> onServerConnected    = {},
        std::function<void( const std::string& ipv4, int port )> onServerDisconnected = {},
        std::function<void( const std::string& streamName, const hub::Acquisition& )>
            onNewAcquisition = {},
        QWidget* parent      = nullptr );
    ~FormServerView();

  signals:
    //    void newStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    //    void delStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
        void serverConnected( const std::string& ipv4, int port );
        void serverDisconnected( const std::string& ipv4, int port );

  private slots:
    //    bool onNewStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    //    void onDelStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
        void onServerConnected( const std::string& ipv4, int port );
        void onServerDisconnected( const std::string& ipv4, int port );

  private slots:
    void on_lineEdit_ip_textChanged( const QString& arg1 );
    void on_spinBox_port_valueChanged( int arg1 );

  private:
    Ui::FormServerView* ui;

    const QRegularExpression m_ipv4RegularExpression =
        QRegularExpression( "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}" );

    hub::Viewer* m_viewer = nullptr;

//    std::string m_ipv4;
//    int m_port;

  public:
    //    void setPort( int newPort );
    const int& getPort() const;
    const std::string& getIpv4() const;
};
