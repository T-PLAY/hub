#ifndef SENSORVIEWS_H
#define SENSORVIEWS_H

#include <QObject>
#include <QThread>

#include <FormSensorView.h>
#include <MainWindowStreamView.h>
#include <QMdiArea>
#include <QVBoxLayout>
#include <map>

class Thread_Client : public QThread
{
    Q_OBJECT
  public:
    // constructor
    explicit Thread_Client( QObject* parent = 0 );

  signals:
    void addSensorSignal( std::string streamerSensorName,
                          std::string format,
                          std::string dims,
                          std::string size,
                          std::string metaData );
    void delSensorSignal( std::string streamerSensorName );

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
    void addSensor( std::string streamerSensorName,
                    std::string format,
                    std::string dims,
                    std::string size,
                    std::string metaData );
    void delSensor( std::string streamerSensorName );

    void addStreamView( std::string streamerSensorName );
    void delStreamView( std::string streamerSensorName );

    void onCloseStreamView( std::string streamerSensorName );

  private:
    Thread_Client mThreadClient;
    std::map<std::string, MainWindowStreamView*> mStreamViews;
    std::map<std::string, FormSensorView*> mSensorViews;

    QVBoxLayout & m_vBoxLayout;
    QMdiArea & m_mdiArea;
};

#endif // SENSORVIEWS_H
