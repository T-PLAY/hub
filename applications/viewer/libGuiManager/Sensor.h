#pragma once

#include <mutex>

#include <QObject>
#include <QThread>

#include <Gui/Viewer/Viewer.hpp>

#include <QMdiArea>
#include <QStandardItem>
#include <WidgetStreamView.h>

#include <Dof6Component.hpp>
#include <ScanComponent.hpp>
#include <SensorComponent.h>

#ifdef ENABLE_IMAGE_VIEWER
#    include <FormImageManipulator.h>
#endif
#include <WidgetStreamView.h>

#include <InputSensor.hpp>

class Sensor;

class SensorThread : public QThread
{
    Q_OBJECT
  public:
    // constructor

    SensorThread( Sensor& sensor, QObject* parent = nullptr );

    ~SensorThread();

  signals:

  public:
    // overriding the QThread's run() method
    void run();

  private:
    Sensor& m_sensor;
};

///////////////////////////////////////////////////////////////////////////////////

class SensorCounterFpsThread : public QThread
{
    Q_OBJECT
  public:
    SensorCounterFpsThread( Sensor& sensor, QObject* parent = nullptr );
    ~SensorCounterFpsThread();
    // constructor
  signals:

  public:
    // overriding the QThread's run() method
    void run();

  private:
    Sensor& m_sensor;
};

////////////////////////////////////////////////////////////////////////

class Sensor : public QObject
{
    Q_OBJECT
  public:
//    Sensor( std::unique_ptr<hub::InputSensor> inputSensor,
//            QMdiArea& mdiArea,
//            Ra::Engine::RadiumEngine* engine,
//            Ra::Gui::Viewer* viewer,
//            Ra::Engine::Scene::System* sys,
//            Sensor* parentSensor,
//            const std::string& streamName,
//            QStandardItemModel& model,
//            QTableView& view,
//            QObject* parent = nullptr );

    Sensor( const hub::SensorSpec & sensorSpec,
            QMdiArea& mdiArea,
            Ra::Engine::RadiumEngine* engine,
            Ra::Gui::Viewer* viewer,
            Ra::Engine::Scene::System* sys,
            Sensor* parentSensor,
            const std::string& streamName,
            QStandardItemModel& model,
            QTableView& view,
            QObject* parent = nullptr );

    ~Sensor();

    void updateTransform( const Ra::Engine::Scene::Entity* entity );
    void update(const hub::Acquisition & acq);

#ifdef ENABLE_IMAGE_VIEWER
    void detachFromImageManipulator();
    void attachFromImageManipulator();
#endif
    void fitView();
    void setParent( Sensor* parent );

    void onTransparencyChanged( double transparency );
    void onTransparency2Changed( double transparency );

    void on_tune_valueChanged( double arg1 );
    void on_tune2_valueChanged( double arg1 );
    void on_tune3_valueChanged( double arg1 );
    void on_tune4_valueChanged( double arg1 );
    void on_palette_valueChanged( int palette );
    void on_setTransparency( bool isTransparent );

  signals:

  protected:
    friend class SensorThread;
    friend class SensorCounterFpsThread;

  public:
//    std::unique_ptr<hub::InputSensor> m_inputSensor;
    const hub::SensorSpec m_sensorSpec;

    Ra::Engine::RadiumEngine* m_engine = nullptr;
    Ra::Gui::Viewer* m_viewer          = nullptr;
    Ra::Engine::Scene::System* m_sys   = nullptr;

#ifdef ENABLE_IMAGE_VIEWER
    FormImageManipulator* m_imageManipulator = nullptr;
#endif
  private:
    QMdiArea& m_mdiArea;
    WidgetStreamView2D* m_widgetStreamViewManipulator = nullptr;

    std::vector<WidgetStreamView*> m_widgetStreamViews;
    std::vector<QMdiSubWindow*> m_subWindows;
    QList<QStandardItem*> m_items;
    QStandardItem* m_itemFps = nullptr;

    SensorThread m_sensorThread;
    SensorCounterFpsThread m_counterFpsThread;
    int m_counterFrame = 0;

    // Create and initialize entity and component
    Ra::Engine::Scene::Entity* m_entity = nullptr;
    Sensor* m_parent                    = nullptr;
    std::list<Sensor*> m_sons;

    std::function<void( const Ra::Engine::Scene::Entity* entity )> m_observer;
    int m_observerId = -1;

    SensorComponent* m_component = nullptr;
    QStandardItemModel& m_model;
    QTableView& m_view;

    bool m_lost = false;
    std::mutex m_mtxUpdating;

  public:
    const QList<QStandardItem*>& getItems() const;
    Ra::Engine::Scene::Component* getComponent();
    SensorComponent* getSensorComponent();
    Ra::Engine::Scene::Entity* getEntity() const;
};
