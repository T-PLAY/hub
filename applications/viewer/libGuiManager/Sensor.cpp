#include "Sensor.h"

//#include <QM
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

            std::cout << "[SensorThread] receive acq : " << acq << std::endl;
            //            mAcqs.push(std::move(acq));
            //            mAcqs.push({});
            //            *mInputStream >> mAcqs.back();
            //            mAcqs.push(acq.clone());

            assert(m_sensor.m_widgetStreamView != nullptr);
            m_sensor.m_widgetStreamView->setData((unsigned char*)acq.mData, inputStream->getAcquisitionSize(), inputStream->getDims(), inputStream->getFormat());

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

Sensor::Sensor(std::unique_ptr<InputStream> inputStream, QMdiArea& mdiArea, QObject* parent)
    : QObject(parent)
    , m_mdiArea(mdiArea)
    //    , m_inputStream(std::move(inputStream))
    , m_inputStream(std::move(inputStream))
    , m_thread(*this, parent)
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

    //    m_thread.run();
    m_thread.start();
}

Sensor::~Sensor()
{
    std::cout << "[Sensor] ~Sensor(" << m_inputStream->getSensorName() << ")" << std::endl;

    if (m_thread.isRunning()) {
        m_thread.requestInterruption();
        m_thread.wait();
    }
    assert(m_widgetStreamView != nullptr);
    //    m_mdiArea.removeSubWindow(m_widgetStreamView->parentWidget());
    m_mdiArea.removeSubWindow(m_subWindow);
    delete m_widgetStreamView;
    m_widgetStreamView = nullptr;
    delete m_subWindow;
    m_subWindow = nullptr;
}
