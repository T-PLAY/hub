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

class Thread_Client : public QThread
{
    Q_OBJECT
  public:
    // constructor
    explicit Thread_Client( QObject* parent = 0 );

  signals:
    void addSensorSignal( std::string sensorName,
                          std::string format,
                          std::string dims,
                          std::string size,
                          std::string metaData );
    void delSensorSignal( std::string sensorName );

  public:
    // overriding the QThread's run() method
    void run();

  private:
};


class SensorViews : public QObject
{
    Q_OBJECT
  public:
    SensorViews( QBoxLayout& vboxLayout, QMdiArea& mdiArea, QObject* parent = nullptr );
    ~SensorViews();

  signals:
    void streamingStarted(std::string sensorName);
    void streamingStopped(std::string sensorName);
//    void startStreaming();
//    void stopStreaming();
//    void sensorAdded(const std::string & sensorName);
//    void sensorDeleted(const std::string & sensorName);

  public slots:
    void addSensor( std::string sensorName,
                    std::string format,
                    std::string dims,
                    std::string size,
                    std::string metaData );
    void delSensor( std::string sensorName );

    const FormSensorView & getSensorView(const std::string & sensorName) const;


  private:
    friend class Thread_Client;
    Thread_Client mThreadClient;

//    Thread_InputStream mThread;
//    std::map<std::string, SensorView*> m_sensorViews;
//    std::map<std::string, MainWindowStreamView*> mStreamViews;
    std::map<std::string, FormSensorView*> m_sensorViews;

    QBoxLayout & m_vBoxLayout;
    QMdiArea & m_mdiArea;
//    QComboBox & m_comboBoxScan;
//    QComboBox & m_comboBoxPose;
};

#endif // SENSORVIEWS_H
