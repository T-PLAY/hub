#include "Sensor.h"

//#include <QM
#include <Engine/Scene/EntityManager.hpp>
#include <QMdiSubWindow>
#include <WidgetStreamView.h>

SensorThread::SensorThread(Sensor& sensor, QObject* parent)
    : QThread { parent }
    , m_sensor(sensor)
{
}

SensorThread::~SensorThread()
{
    std::cout << "[SensorThread] ~SensorThread(" << m_sensor.m_inputStream->getSensorName() << ")" << std::endl;

    //    assert(! isRunning());
    //    if (this->isRunning()) {
    //        this->requestInterruption();
    //        this->wait();
    //    }
}

void SensorThread::run()
{
    std::cout << "[SensorThread] run()" << std::endl;

    auto& inputStream = m_sensor.m_inputStream;

    try {

        while (!this->isInterruptionRequested()) {

            //            Stream::Acquisition acq;
            //            *mInputStream >> acq;
            const auto& acq = inputStream->getAcquisition();
            //            std::cout << "[SensorThread] receive acq : " << acq << std::endl;

            //            mAcqs.push(std::move(acq));
            //            mAcqs.push({});
            //            *mInputStream >> mAcqs.back();
            //            mAcqs.push(acq.clone());

            // update 2D view
            {
                assert(m_sensor.m_widgetStreamView != nullptr);
                m_sensor.m_widgetStreamView->setData((unsigned char*)acq.mData, inputStream->getAcquisitionSize(), inputStream->getDims(), inputStream->getFormat());
            }

            // update 3D component
            {
                assert(m_sensor.m_component != nullptr);
                m_sensor.m_component->update(acq);
            }

            ++m_sensor.m_counterFrame;

            //            mAcqs.emplace()
            //            std::cout << "[SensorThread] acqs back : " << mAcqs.front() <<
            //            std::endl; mAcqs.emplace(acq); mAcqs.push(std::move(acq));

            //            emit newAcquisition(mSourceType);
        }
    } catch (std::exception& e) {
        std::cout << "[SensorThread] catch exception : " << e.what() << std::endl;
        //        std::cout << "[SensorThread] streamingStopped : " << mSourceType <<
        //        std::endl;
        //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        //        emit streamingStopped(mSourceType);
        //        mFormInputStreamViews.onKillInputStreamPrivate(mSensorName);
        return;
    }

    std::cout << "[SensorThread] run() end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////

SensorCounterFpsThread::SensorCounterFpsThread(Sensor& sensor, QObject* parent)
    : QThread(parent)
    , m_sensor(sensor)
{
}

SensorCounterFpsThread::~SensorCounterFpsThread()
{
//    if (this->isRunning()) {
//        this->requestInterruption();
//        this->wait();
//    }
}

void SensorCounterFpsThread::run()
{

    //    double fps = 0.0;
    //    m_labelFps.setText("0 Hz");

    while (!this->isInterruptionRequested()) {

        int i = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        while (!this->isInterruptionRequested() && i < 10) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ++i;
        }
        std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        char buff[32];
        double fps = 1000.0 * m_sensor.m_counterFrame / duration;
        sprintf(buff, "%.2f Hz", fps);
        //        m_labelFps.setText((std::string(buff) + " Hz").c_str());
        m_sensor.m_itemFps->setText(QString(buff));
        //        m_counterFrame = 0;
        m_sensor.m_counterFrame = 0;

        //        std::cout << "[SensorCounterFpsThread:" << m_sensorName << "] update" << std::endl;
        //        double fps = m_counterFrame * 10.0;
    }
    //    assert(false);
}

/////////////////////////////////////////////////////////////////////

// Sensor::Sensor(IOStream&& iostream, QObject* parent)
//     : QObject { parent }
//     , m_inputStream(std::move(iostream))
//{
// }

// Sensor::Sensor(IOStream &&iostream, QObject *parent)
//     :QObject{parent}
//     , m_inputStream(new InputStream(std::move(iostream)))
//{

//}

Sensor::Sensor(std::unique_ptr<InputStream> inputStream, QMdiArea& mdiArea, Ra::Engine::RadiumEngine* engine, Ra::Gui::Viewer* viewer, Ra::Engine::Scene::System* sys, QObject* parent)
    : QObject(parent)
    , m_inputStream(std::move(inputStream))
    , m_engine(engine)
    , m_viewer(viewer)
    , m_sys(sys)
    , m_mdiArea(mdiArea)
    //    , m_inputStream(std::move(inputStream))
    , m_thread(*this, parent)
    , m_counterFpsThread(*this, parent)
{

    // mdiArea window
    {
        assert(m_widgetStreamView == nullptr);

        const auto& dims = m_inputStream->getDims();
        if (dims.size() == 1) {
            m_widgetStreamView = new WidgetStreamView1D(&m_mdiArea);
            m_widgetStreamView->setMinimumSize(400, 35);

        } else if (dims.size() == 2) {
            m_widgetStreamView = new WidgetStreamView2D(&m_mdiArea);
            m_widgetStreamView->setMinimumWidth(dims.at(0));
            m_widgetStreamView->setMinimumHeight(dims.at(1));
        }
        //        if (m_widgetStreamView != nullptr) {
        assert(m_widgetStreamView != nullptr);

        m_subWindow = m_mdiArea.addSubWindow(m_widgetStreamView);
        m_subWindow->setVisible(true);
        m_subWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowTitleHint);

        std::string dimText = "(dim: ";
        for (int i = 0; i < dims.size(); ++i) {
            dimText += std::to_string(dims.at(i));
            if (i != dims.size() - 1) {
                dimText += " x ";
            }
        }
        dimText += ")";

        std::string formatText = std::string("(format: ") + Stream::format2string[(int)m_inputStream->getFormat()] + ")";
        m_subWindow->setWindowTitle((m_inputStream->getSensorName() + "   " + dimText + "   " + formatText).c_str());
        //        }
    }

    // create 3D scene object
    {
        assert(m_component == nullptr);
        m_entity = m_engine->getEntityManager()->createEntity(m_inputStream->getSensorName() + " entity");
        switch (m_inputStream->getFormat()) {
        case Stream::Format::DOF6:
            //            assert(m_dof6Component == nullptr);
            //            m_dof6Component = new Dof6Component(m_entity);
            m_component = new Dof6Component(*m_inputStream, m_entity);
            break;

        case Stream::Format::Y8:
            m_component = new ScanComponent(*m_inputStream, m_entity, *m_engine, *m_viewer);
            break;

        default:
            assert(false);
        }
        //    Ra::Engine::Scene::Component* c = new Dof6Component(e);
        m_sys->addComponent(m_entity, m_component);
        m_component->initialize();
        //        m_viewer->glInitialized();
        m_viewer->prepareDisplay();
    }

    m_items.append(new QStandardItem(m_inputStream->getSensorName().c_str()));
    m_items.append(new QStandardItem(Stream::format2string[(int)m_inputStream->getFormat()]));
    m_items.append(new QStandardItem(Stream::dims2string(m_inputStream->getDims()).c_str()));
    m_items.append(new QStandardItem(std::to_string(m_inputStream->getAcquisitionSize()).c_str()));
    m_itemFps = new QStandardItem("0");
    m_items.append(m_itemFps);

    //    m_thread.run();
    m_counterFpsThread.start();
    m_thread.start();
}

Sensor::~Sensor()
{
    std::cout << "[Sensor] ~Sensor(" << m_inputStream->getSensorName() << ")" << std::endl;

    if (m_thread.isRunning()) {
        m_thread.requestInterruption();
        m_thread.wait();
    }
    if (m_counterFpsThread.isRunning()) {
        m_counterFpsThread.requestInterruption();
        m_counterFpsThread.wait();
    }

    // 3D
    m_engine->getEntityManager()->removeEntity(m_entity);
    //    delete m_component;
    m_component = nullptr;
    //    delete m_entity;
    m_entity = nullptr;

    // 2D
    assert(m_widgetStreamView != nullptr);
    //    m_mdiArea.removeSubWindow(m_widgetStreamView->parentWidget());
    m_mdiArea.removeSubWindow(m_subWindow);
    delete m_widgetStreamView;
    m_widgetStreamView = nullptr;
    delete m_subWindow;
    m_subWindow = nullptr;
}

///////////////////////////////////////////////////////

const QList<QStandardItem*>& Sensor::getItems() const
{
    return m_items;
}

Ra::Engine::Scene::Component* Sensor::getComponent()
{
    assert(m_component != nullptr);
    return m_component;
    //    switch (m_inputStream->getFormat()) {
    //    case Stream::Format::DOF6:
    //        assert(m_dof6Component != nullptr);
    //        return m_dof6Component;
    //        break;

    //    default:
    //        assert(false);
    //    }
}
