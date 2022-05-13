#ifndef SENSORVIEWS_H
#define SENSORVIEWS_H

#include <QObject>
#include <QThread>

#include <FormSensorView.h>
#include <MainWindowStreamView.h>
#include <QMdiArea>
#include <QVBoxLayout>
#include <map>
#include <stream.h>
#include <SensorView.h>
#include <QComboBox>

#include <DialogServerConnect.h>

class Thread_Client : public QThread
{
    Q_OBJECT
  public:
    // constructor
    explicit Thread_Client( DialogServerConnect & dialog,  QObject* parent = 0 );

  signals:
    void serverConnected();
    void serverDisconnected();
    void addSensorSignal( std::string sensorName,
                          std::string format,
                          std::string dims,
                          std::string size,
                          std::string metaData );
    void delSensorSignal( std::string sensorName );

  public:
    // overriding the QThread's run() method
    void run();

    DialogServerConnect& m_dialog;

  private:
};


class SensorViews : public QObject
{
    Q_OBJECT
  public:
    SensorViews( QBoxLayout& vboxLayout, QMdiArea& mdiArea, QMainWindow & mainWindow, QObject* parent = nullptr );
    ~SensorViews();

  signals:
//    void serverConnected();
//    void serverDisconnected();
    void streamingStarted(std::string sensorName);
    void streamingStopped(std::string sensorName);
//    void startStreaming();
//    void stopStreaming();
//    void sensorAdded(const std::string & sensorName);
//    void sensorDeleted(const std::string & sensorName);

  public slots:
    void onServerConnect();
    void onServerDisconnect();
    void addSensor( std::string sensorName,
                    std::string format,
                    std::string dims,
                    std::string size,
                    std::string metaData );
    void delSensor( std::string sensorName );
//    void onQuitApp();

    const FormSensorView & getSensorView(const std::string & sensorName) const;



  private:
    DialogServerConnect m_dialog;

    friend class Thread_Client;
    Thread_Client mThreadClient;

//    Thread_InputStream mThread;
//    std::map<std::string, SensorView*> m_sensorViews;
//    std::map<std::string, MainWindowStreamView*> mStreamViews;
    std::map<std::string, FormSensorView*> m_sensorViews;

    QBoxLayout & m_vBoxLayout;
    QMdiArea & m_mdiArea;
    QMainWindow & m_mainWindow;

//    DialogServerConnect & m_dialog;
//    QComboBox & m_comboBoxScan;
//    QComboBox & m_comboBoxPose;
};

#endif // SENSORVIEWS_H
