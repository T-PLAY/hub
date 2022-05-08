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
    SensorViews( QVBoxLayout& vboxLayout, QMdiArea& mdiArea, QObject* parent = nullptr );
    ~SensorViews();

  signals:

  public slots:
    void addSensor( std::string sensorName,
                    std::string format,
                    std::string dims,
                    std::string size,
                    std::string metaData );
    void delSensor( std::string sensorName );


  private:
    friend class Thread_Client;
    Thread_Client mThreadClient;

//    Thread_InputStream mThread;
//    std::map<std::string, SensorView*> m_sensorViews;
//    std::map<std::string, MainWindowStreamView*> mStreamViews;
    std::map<std::string, FormSensorView*> m_sensorViews;

    QVBoxLayout & m_vBoxLayout;
    QMdiArea & m_mdiArea;
};

#endif // SENSORVIEWS_H
