#ifndef SENSOR_H
#define SENSOR_H

#include <IOStream.h>
#include <QObject>
#include <QThread>
#include <stream.h>

#include <Gui/Viewer/Viewer.hpp>

#include <QMdiArea>
#include <WidgetStreamView.h>
#include <QStandardItem>

#include <Dof6Component.hpp>
#include <ScanComponent.hpp>
#include <SensorComponent.h>

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
    Sensor(std::unique_ptr<InputStream> inputStream, QMdiArea& mdiArea, Ra::Engine::RadiumEngine * engine, Ra::Gui::Viewer * viewer, Ra::Engine::Scene::System * sys, Ra::Engine::Scene::Entity * parentEntity, QObject* parent = nullptr);
    ~Sensor();

    //    Sensor(IOStream&& iostream, QObject* parent = nullptr);
    void updateTransform(const Ra::Engine::Scene::Entity * entity);

//    Ra::Engine::Scene::Entity* m_entity = nullptr;

signals:

protected:
    friend class SensorThread;
    friend class SensorCounterFpsThread;

public:
    std::unique_ptr<InputStream> m_inputStream;

    Ra::Engine::RadiumEngine * m_engine = nullptr;
    Ra::Gui::Viewer * m_viewer = nullptr;
    Ra::Engine::Scene::System * m_sys = nullptr;

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

    // Create and initialize entity and component
    Ra::Engine::Scene::Entity* m_entity = nullptr;
    Ra::Engine::Scene::Entity* m_parentEntity = nullptr;
//    Ra::Engine::Scene::Component * m_component = nullptr;
//    Dof6Component * m_dof6Component = nullptr;
    SensorComponent * m_component = nullptr;

public:
    const QList<QStandardItem *> &getItems() const;
    Ra::Engine::Scene::Component * getComponent();
    Ra::Engine::Scene::Entity *getEntity() const;
};

// Sensor::Sensor(IOStreamT&& iostream, QObject* parent)
// template <class IOStreamT>
// Sensor::Sensor(IOStreamT&& iostream, QObject* parent)
//     : QObject(parent)
//     , m_inputStream(new InputStream(std::move(iostream)))
//{
// }

#endif // SENSOR_H
