#pragma once

#include <QObject>
#include <QThread>

#include <Gui/Viewer/Viewer.hpp>

#include <QMdiArea>
#include <WidgetStreamView.h>
#include <QStandardItem>

#include <Dof6Component.hpp>
#include <ScanComponent.hpp>
#include <StreamComponent.h>

#include <FormImageManipulator.h>
#include <WidgetStreamView.h>

#include <InputSensor.hpp>

class Sensor;

class SensorThread : public QThread {
    Q_OBJECT
public:
    // constructor

    //    template <class IOStreamT>
    //    SensorThread(IOStreamT&& iostream, QObject* parent = nullptr);
    SensorThread(Sensor & sensor, QObject* parent = nullptr);
    //    SensorThread(hub::InputSensor& iostream, FormInputStreamViews& formInputStreamViews, int iSensor, QObject* parent = nullptr);

    ~SensorThread();

signals:
    //     void newAcquisition(const std::string& sourceType);
    //     void streamingStopped(const std::string& sourceType);

public:
    // overriding the QThread's run() method
    void run();

    //    std::queue<Sensor::Acquisition> mAcqs;

    //    std::unique_ptr<hub::InputSensor> mInputStream;

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

    //    Sensor(hub::InputSensor&& inputSensor, QObject* parent = nullptr);
    Sensor(std::unique_ptr<hub::InputSensor> inputSensor, QMdiArea& mdiArea, FormImageManipulator * imageManipulator, Ra::Engine::RadiumEngine * engine, Ra::Gui::Viewer * viewer, Ra::Engine::Scene::System * sys, Sensor * parentSensor, QObject* parent = nullptr);
    ~Sensor();

    //    Sensor(IOStream&& iostream, QObject* parent = nullptr);
    void updateTransform(const Ra::Engine::Scene::Entity * entity);

    void detachFromImageManipulator();
    void attachFromImageManipulator();
//    Ra::Engine::Scene::Entity* m_entity = nullptr;
    void setParent(Sensor * parent);

signals:

protected:
    friend class SensorThread;
    friend class SensorCounterFpsThread;

public:
    std::unique_ptr<hub::InputSensor> m_inputSensor;

    Ra::Engine::RadiumEngine * m_engine = nullptr;
    Ra::Gui::Viewer * m_viewer = nullptr;
    Ra::Engine::Scene::System * m_sys = nullptr;

private:
    //    hub::InputSensor m_inputStream;
    QMdiArea& m_mdiArea;
    FormImageManipulator* m_imageManipulator = nullptr;
    WidgetStreamView2D * m_widgetStreamViewManipulator = nullptr;

    WidgetStreamView* m_widgetStreamView = nullptr;
    QMdiSubWindow* m_subWindow = nullptr;
    QList<QStandardItem*> m_items;
    QStandardItem * m_itemFps = nullptr;

    SensorThread m_thread;
    SensorCounterFpsThread m_counterFpsThread;
    int m_counterFrame = 0;

    // Create and initialize entity and component
    Ra::Engine::Scene::Entity* m_entity = nullptr;
//    Ra::Engine::Scene::Entity* m_parentEntity = nullptr;
    Sensor * m_parent = nullptr;
    std::list<Sensor*> m_sons;

    std::function<void(const Ra::Engine::Scene::Entity* entity)> m_observer;
    int m_observerId = -1;

//    Ra::Engine::Scene::Component * m_component = nullptr;
//    Dof6Component * m_dof6Component = nullptr;
    StreamComponent * m_component = nullptr;

public:
    const QList<QStandardItem *> &getItems() const;
    Ra::Engine::Scene::Component * getComponent();
    Ra::Engine::Scene::Entity *getEntity() const;
};

// Sensor::Sensor(IOStreamT&& iostream, QObject* parent)
// template <class IOStreamT>
// Sensor::Sensor(IOStreamT&& iostream, QObject* parent)
//     : QObject(parent)
//     , m_inputStream(new hub::InputSensor(std::move(iostream)))
//{
// }

