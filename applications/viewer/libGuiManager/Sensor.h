#pragma once

#include <QObject>
#include <QThread>

#include <Gui/Viewer/Viewer.hpp>

#include <QMdiArea>
#include <WidgetStreamView.h>
#include <QStandardItem>

#include <Dof6Component.hpp>
#include <ScanComponent.hpp>
#include <SensorComponent.h>

#ifdef ENABLE_IMAGE_VIEWER
#include <FormImageManipulator.h>
#endif
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
//    Sensor(std::unique_ptr<hub::InputSensor> inputSensor, QMdiArea& mdiArea, FormImageManipulator * imageManipulator, Ra::Engine::RadiumEngine * engine, Ra::Gui::Viewer * viewer, Ra::Engine::Scene::System * sys, Sensor * parentSensor, const std::string & streamName, QStandardItemModel & model, QTableView & view, QObject* parent = nullptr);
    Sensor(std::unique_ptr<hub::InputSensor> inputSensor, QMdiArea& mdiArea, Ra::Engine::RadiumEngine * engine, Ra::Gui::Viewer * viewer, Ra::Engine::Scene::System * sys, Sensor * parentSensor, const std::string & streamName, QStandardItemModel & model, QTableView & view, QObject* parent = nullptr);
    ~Sensor();

    //    Sensor(IOStream&& iostream, QObject* parent = nullptr);
    void updateTransform(const Ra::Engine::Scene::Entity * entity);

#ifdef ENABLE_IMAGE_VIEWER
    void detachFromImageManipulator();
    void attachFromImageManipulator();
#endif
    void fitView();
//    Ra::Engine::Scene::Entity* m_entity = nullptr;
    void setParent(Sensor * parent);

    void onTransparencyChanged(double transparency);
    void onTransparency2Changed(double transparency);

    void on_tune_valueChanged(double arg1);
    void on_tune2_valueChanged(double arg1);
    void on_tune3_valueChanged(double arg1);
    void on_tune4_valueChanged(double arg1);
    void on_palette_valueChanged(int palette);
    void on_setTransparency(bool isTransparent);

signals:

protected:
    friend class SensorThread;
    friend class SensorCounterFpsThread;

public:
    std::unique_ptr<hub::InputSensor> m_inputSensor;

    Ra::Engine::RadiumEngine * m_engine = nullptr;
    Ra::Gui::Viewer * m_viewer = nullptr;
    Ra::Engine::Scene::System * m_sys = nullptr;

#ifdef ENABLE_IMAGE_VIEWER
    FormImageManipulator* m_imageManipulator = nullptr;
#endif
private:
    //    hub::InputSensor m_inputStream;
    QMdiArea& m_mdiArea;
    WidgetStreamView2D * m_widgetStreamViewManipulator = nullptr;

    std::vector<WidgetStreamView*> m_widgetStreamViews;
    std::vector<QMdiSubWindow*> m_subWindows;
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
    SensorComponent * m_component = nullptr;
    QStandardItemModel & m_model;
    QTableView & m_view;

public:
    const QList<QStandardItem *> &getItems() const;
    Ra::Engine::Scene::Component * getComponent();
    SensorComponent * getSensorComponent();
    Ra::Engine::Scene::Entity *getEntity() const;
};

// Sensor::Sensor(IOStreamT&& iostream, QObject* parent)
// template <class IOStreamT>
// Sensor::Sensor(IOStreamT&& iostream, QObject* parent)
//     : QObject(parent)
//     , m_inputStream(new hub::InputSensor(std::move(iostream)))
//{
// }

