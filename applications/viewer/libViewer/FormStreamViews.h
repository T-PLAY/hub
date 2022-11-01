#ifndef FORMSENSORVIEWS_H
#define FORMSENSORVIEWS_H

#include <QThread>
#include <QWidget>

#include <FormStreamView.h>
#include <MainWindowStreamView.h>
#include <QComboBox>
#include <QMdiArea>
#include <QVBoxLayout>
//#include <SensorView.h>
#include <map>

#include <QStringListModel>

#include <Viewer.hpp>

//#include <DialogServerConnect.h>

class FormStreamViews;

class ViewerQt : public QObject
{
    Q_OBJECT
  public:
    ViewerQt( const std::string& ipv4, const int& port );
    ~ViewerQt();
  signals:
    void serverConnected();
    void serverDisconnected();
    void addStreamSignal( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void delStreamSignal( const std::string& streamName, const hub::SensorSpec& sensorSpec );

  public:
    // private:
    hub::Viewer* m_viewer = nullptr;
};

// class Thread_Client : public QThread {
// public:

// signals:

// public:


// private:
// };

namespace Ui {
class FormStreamViews;
}

class FormStreamViews : public QWidget
{
    Q_OBJECT

  public:
    explicit FormStreamViews( QWidget* parent = nullptr );
    ~FormStreamViews();



  signals:
    void streamingStarted( const std::string& streamName, const std::string& syncSensorName );
    void streamingStopped( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void serverDisconnected();
    void serverConnected();

  public slots:
    void onServerConnect();
    void onServerDisconnect();
    void addStream( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void delStream( const std::string& streamName, const hub::SensorSpec& sensorSpec );

  private:
    Ui::FormStreamViews* ui;



    ViewerQt* m_viewerQt = nullptr;
    std::string m_ipv4;
    int m_port;
    const QRegularExpression m_ipv4RegularExpression =
        QRegularExpression( "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}" );


    std::map<std::string, FormStreamView*> m_sensorViews;
    QStringListModel m_sensorModel;


    bool m_serverConnected = false;
    bool m_serverPing      = false;

    bool m_autoStartStream = true;

  public:
    const FormStreamView& getSensorView( const std::string& streamName ) const;

    const std::string& getIpv4() const;
    const int& getPort() const;

  private slots:
    void on_lineEdit_ip_textChanged( const QString& ipv4 );
    void on_spinBox_port_valueChanged( int port );
};

#endif // FORMSENSORVIEWS_H
