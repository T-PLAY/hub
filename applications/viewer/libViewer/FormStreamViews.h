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
    //        std::string format,
    //        std::string dims,
    //        std::string size,
    //        std::string metaData);
    void delStreamSignal( const std::string& streamName, const hub::SensorSpec& sensorSpec );

  public:
    // private:
    hub::Viewer* m_viewer = nullptr;
};

// class Thread_Client : public QThread {
//     Q_OBJECT
// public:
//     // constructor
//     explicit Thread_Client(const FormStreamViews &formSensorViews, QObject* parent = 0);

// signals:
//     void serverConnected();
//     void serverDisconnected();
//     void addStreamSignal(std::string streamName,
//         const hub::SensorSpec & sensorSpec);
////        std::string format,
////        std::string dims,
////        std::string size,
////        std::string metaData);
//    void delStreamSignal(std::string streamName, const hub::SensorSpec & sensorSpec);

// public:
//     // overriding the QThread's run() method
//     void run();

////    DialogServerConnect& m_dialog;
//    const FormStreamViews & m_formSensorViews;

// private:
// };

namespace Ui {
class FormStreamViews;
}

class FormStreamViews : public QWidget
{
    Q_OBJECT

  public:
    //    FormStreamViews(QBoxLayout& vboxLayout, QMdiArea& mdiArea, QMainWindow& mainWindow,
    //    QWidget* parent = nullptr);
    explicit FormStreamViews( QWidget* parent = nullptr );
    ~FormStreamViews();

    //    void closeAllStream();

    //    void startStreaming();

  signals:
    //    void isServerConnected();
    //    void serverDisconnected();
    void streamingStarted( const std::string& streamName, const std::string& syncSensorName );
    void streamingStopped( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    //    void startStreaming();
    //    void stopStreaming();
    //    void sensorAdded(const std::string & streamName);
    //    void sensorDeleted(const std::string & streamName);
    void serverDisconnected();
    void serverConnected();

  public slots:
    void onServerConnect();
    void onServerDisconnect();
    void addStream( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void delStream( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    //    void onQuitApp();

  private:
    Ui::FormStreamViews* ui;

    //    DialogServerConnect m_dialog;

    //    friend class Thread_Client;
    //    Thread_Client mThreadClient;
    //    hub::Viewer * m_viewer;

    ViewerQt* m_viewerQt = nullptr;
    //    hub::Viewer * m_viewer = nullptr;
    std::string m_ipv4;
    int m_port;
    const QRegularExpression m_ipv4RegularExpression =
        QRegularExpression( "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}" );

    //    Thread_InputStream mThread;
    //    std::map<std::string, SensorView*> m_sensorViews;
    //    std::map<std::string, MainWindowStreamView*> mStreamViews;

    std::map<std::string, FormStreamView*> m_sensorViews;
    QStringListModel m_sensorModel;

    //    QBoxLayout& m_vBoxLayout;
    //    QMdiArea * m_mdiArea = nullptr;
    //    QMainWindow& m_mainWindow;

    bool m_serverConnected = false;
    bool m_serverPing      = false;
    //    DialogServerConnect & m_dialog;
    //    QComboBox & m_comboBoxScan;
    //    QComboBox & m_comboBoxPose;

    bool m_autoStartStream = true;
    //    bool m_autoStartStream = false;

  public:
    const FormStreamView& getSensorView( const std::string& streamName ) const;
    //    void setMdiArea(QMdiArea *newMdiArea);

    //    bool isServerConnected() const;
    const std::string& getIpv4() const;
    const int& getPort() const;

  private slots:
    void on_lineEdit_ip_textChanged( const QString& ipv4 );
    void on_spinBox_port_valueChanged( int port );
};

#endif // FORMSENSORVIEWS_H
