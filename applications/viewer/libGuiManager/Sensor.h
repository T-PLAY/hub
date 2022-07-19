#ifndef SENSOR_H
#define SENSOR_H

#include <IOStream.h>
#include <QObject>
#include <QThread>
#include <stream.h>

#include <QMdiArea>
#include <WidgetStreamView.h>
#include <QStandardItem>

class Sensor;

class SensorThread : public QThread {
    Q_OBJECT
public:
    // constructor

    //    template <class IOStreamT>
    //    SensorThread(IOStreamT&& iostream, QObject* parent = nullptr);
    SensorThread(Sensor & sensor, QObject* parent = nullptr);
    //    SensorThread(InputStream& iostream, FormInputStreamViews& formInputStreamViews, int iSensor, QObject* parent = nullptr);

    ~SensorThread();

signals:
    //     void newAcquisition(const std::string& sourceType);
    //     void streamingStopped(const std::string& sourceType);

public:
    // overriding the QThread's run() method
    void run();

    //    std::queue<Stream::Acquisition> mAcqs;

    //    std::unique_ptr<InputStream> mInputStream;

private:
    Sensor& m_sensor;
};

///////////////////////////////////////////////////////////////////////////////////

class SensorCounterFpsThread : public QThread {
    Q_OBJECT
public:
    SensorCounterFpsThread(Sensor & sensor, QObject* parent = nullptr);
    ~SensorCounterFpsThread();
    // constructor
signals:

public:
    // overriding the QThread's run() method
    void run();

//    size_t m_counterFrame = 0;

private:
    //    std::string m_sensorName;
    //    double m_fps = 10.0;
    Sensor& m_sensor;
//    QLabel& m_labelFps;
};

////////////////////////////////////////////////////////////////////////

class Sensor : public QObject {
    Q_OBJECT
public:
    //    template <class IOStreamT>
    //    Sensor(IOStreamT&& iostream, QObject* parent = nullptr);

    //    Sensor(InputStream&& inputStream, QObject* parent = nullptr);
    Sensor(std::unique_ptr<InputStream> inputStream, QMdiArea& mdiArea, QObject* parent = nullptr);
    ~Sensor();

    //    Sensor(IOStream&& iostream, QObject* parent = nullptr);

signals:

protected:
    friend class SensorThread;
    friend class SensorCounterFpsThread;

public:
    std::unique_ptr<InputStream> m_inputStream;

private:
    //    InputStream m_inputStream;
    QMdiArea& m_mdiArea;

    WidgetStreamView* m_widgetStreamView = nullptr;
    QMdiSubWindow* m_subWindow = nullptr;
    QList<QStandardItem*> m_items;
    QStandardItem * m_itemFps = nullptr;

    SensorThread m_thread;
    SensorCounterFpsThread m_counterFpsThread;
    int m_counterFrame = 0;

public:
    const QList<QStandardItem *> &getItems() const;
};

// Sensor::Sensor(IOStreamT&& iostream, QObject* parent)
// template <class IOStreamT>
// Sensor::Sensor(IOStreamT&& iostream, QObject* parent)
//     : QObject(parent)
//     , m_inputStream(new InputStream(std::move(iostream)))
//{
// }

#endif // SENSOR_H
