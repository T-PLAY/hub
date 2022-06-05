#include "FormInputStreamView.h"
#include "ui_FormInputStreamView.h"

// InputStreamThread::InputStreamThread(InputStream& inputStream,
//     FormInputStreamViews& formInputStreamViews,
//     int iSensor,
//     QObject* parent)
//     : QThread(parent)
//     , mIStream(iSensor)
//     , mInputStream(inputStream)
//     , mSensorName(inputStream.getSensorName())
//     , mFormInputStreamViews(formInputStreamViews)
////    mInputStream( std::move(iostream))
//{
//    std::cout << "[InputStreamThread] InputStreamThread()" << std::endl;
//}
#include <QMdiSubWindow>

#include <constants.h>

InputStreamThread::InputStreamThread(std::unique_ptr<InputStream> inputStream,
    const std::string& sourceType,
    //    QMdiArea& mdiArea,
    QObject* parent)
    : QThread(parent)
    , mInputStream(std::move(inputStream))
    , mSourceType(sourceType)
//    , m_mdiArea(mdiArea)
{
}

InputStreamThread::~InputStreamThread()
{
    std::cout << "[InputStreamThread] ~InputStreamThread(" << mSourceType << ")" << std::endl;

    //    assert(! isRunning());
    if (this->isRunning()) {
        this->requestInterruption();
        this->wait();
    }
}

void InputStreamThread::run()
{
    std::cout << "[InputStreamThread] run()" << std::endl;

    try {

        while (!this->isInterruptionRequested()) {

            //            Stream::Acquisition acq;
            //            *mInputStream >> acq;
            auto acq = mInputStream->getAcquisition();
            //            std::cout << "[InputStreamThread] receive acq : " << acq << std::endl;
            //            mAcqs.push(std::move(acq));
            //            mAcqs.push({});
            //            *mInputStream >> mAcqs.back();
            mAcqs.push(acq.clone());
            //            mAcqs.emplace()
            //            std::cout << "[InputStreamThread] acqs back : " << mAcqs.front() <<
            //            std::endl; mAcqs.emplace(acq); mAcqs.push(std::move(acq));

            emit newAcquisition(mSourceType);
        }
    } catch (std::exception& e) {
        std::cout << "[InputStreamThread] catch exception : " << e.what() << std::endl;
        //        std::cout << "[InputStreamThread] streamingStopped : " << mSourceType <<
        //        std::endl;
        //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        emit streamingStopped(mSourceType);
        //        mFormInputStreamViews.onKillInputStreamPrivate(mSensorName);
        return;
    }

    std::cout << "[InputStreamThread] run() end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////

CounterFpsThread::CounterFpsThread(QLabel& label)
    : m_labelFps(label)
{
}

CounterFpsThread::~CounterFpsThread()
{
    if (this->isRunning()) {
        this->requestInterruption();
        this->wait();
    }
}

void CounterFpsThread::run()
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
        double fps = 1000.0 * m_counterFrame / duration;
        sprintf(buff, "%.2f", fps);
        m_labelFps.setText((std::string(buff) + " Hz").c_str());
        m_counterFrame = 0;

        //        std::cout << "[CounterFpsThread:" << m_sensorName << "] update" << std::endl;
        //        double fps = m_counterFrame * 10.0;
    }
    //    assert(false);
}

///////////////////////////////////////////////////////////////////////////////////

FormInputStreamView::FormInputStreamView(const std::string& sensorName, QMdiArea& mdiArea, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormInputStreamView)
    , m_sensorName(sensorName)
    , m_mdiArea(mdiArea)
{
    assert(m_sensorName != "");
    ui->setupUi(this);

    ui->label_sensorName->setText(m_sensorName.c_str());

    ui->comboBox_sourceType->setModel(&m_sourceTypeModel);

    //    ui->comboBox_scan->setModel(&m_sourceTypeModel[0]);
    //    ui->comboBox_pose->setModel(&m_sourceTypeModel[1]);
    m_counterFpsThreads = new CounterFpsThread(*ui->label_frequency);
    m_counterFpsThreads->start();
    //    m_counterFpsThreads[0] = new CounterFpsThread(*ui->label_scanFrequency, "scan");
    //    m_counterFpsThreads[0]->start();
}

FormInputStreamView::~FormInputStreamView()
{
    delete m_counterFpsThreads;
    m_counterFpsThreads = nullptr;
    //    delete m_counterFpsThreads[0];
    //    m_counterFpsThreads[0] = nullptr;
    if (m_widgetStreamView != nullptr) {
        m_mdiArea.removeSubWindow(m_widgetStreamView->parentWidget());
        delete m_widgetStreamView;
        m_widgetStreamView = nullptr;
    }

    delete ui;
}

void FormInputStreamView::remove(const std::string& sourceType)
{
    onStreamingStopped(sourceType);
}

Acquisitions& FormInputStreamView::getAcquisitions(const std::string& sourceType)
{
    const auto& activeSourceType = ui->comboBox_sourceType->currentText();
    assert(activeSourceType != "");
    assert(activeSourceType == sourceType.c_str());

    assert(m_sourceType2inputStreamThreads.find(sourceType) != m_sourceType2inputStreamThreads.end());
    //    return m_sourceType2inputStreamThreads.at(sourceType).back()->mAcq;
    //    auto & acqs = m_sourceType2inputStreamThreads.at(sourceType).back()->mAcqs;
    //    auto && acq =
    //    std::move(m_sourceType2inputStreamThreads.at(sourceType).back()->mAcqs.back());
    //    acqs.pop();
    //    return acqs.back();
    return m_sourceType2inputStreamThreads.at(sourceType).back()->mAcqs;
}

// const Stream::Acquisition& FormInputStreamView::getAcquisition(const std::string& sourceType)
// const
//{
//    const auto& activeSourceType = ui->comboBox_sourceType->currentText();
//    assert(activeSourceType != "");
//    assert(activeSourceType == sourceType.c_str());

//    assert(m_sourceType2inputStreamThreads.find(sourceType) !=
//    m_sourceType2inputStreamThreads.end());
////    return m_sourceType2inputStreamThreads.at(sourceType).back()->mAcq;
//    auto & acqs = m_sourceType2inputStreamThreads.at(sourceType).back()->mAcqs;
//    return acqs.back();
//}

const InputStreamThread&
FormInputStreamView::getIputStreamThread(const std::string& sourceType) const
{
    return *m_sourceType2inputStreamThreads.at(sourceType).back().get();
}

// void FormInputStreamView::on_comboBox_sourceType_currentTextChanged(const QString& sourceType)
//{
//     std::cout << "[FormInputStreamView] on_comboBox_sourceType_currentTextChanged('"
//               << sourceType.toStdString() << "')" << std::endl;
// }

void FormInputStreamView::onNewAcquisition(const std::string& sourceType)
{
    const auto& activeSourceType = ui->comboBox_sourceType->currentText();
    assert(activeSourceType != "");
    if (activeSourceType == sourceType.c_str()) {
        //        std::cout << "[FormInputStreamView] onNewAcquisition('" << sourceType << "')" <<
        //        std::endl;

        if (m_widgetStreamView != nullptr) {
            const auto& inputStreamThread = *m_sourceType2inputStreamThreads.at(activeSourceType.toStdString()).front().get();
            const auto& inputStream = inputStreamThread.mInputStream;
            const auto& acqs = inputStreamThread.mAcqs;
            if (acqs.empty())
                return;
            assert(!acqs.empty());
            assert(acqs.back().mData != nullptr);
//            m_widgetStreamView->setData((unsigned char*)acqs.back().mData, inputStream->getDims(), inputStream->getFormat());

//            m_widgetStreamView->setData((unsigned char*)acqs.back().mData, inputStream->getDims(), inputStream->getFormat());
            m_widgetStreamView->setData((unsigned char*)acqs.back().mData, inputStream->getAcquisitionSize(), inputStream->getDims(), inputStream->getFormat());

            //            if (inputStreamThread.mAcqs.size() > 1) {
            //                m_widgetStreamView->setData((unsigned char*)inputStreamThread.mAcqs.back().mData, inputStream->getDims(), inputStream->getFormat());

            //            } else {
            //                m_widgetStreamView->setData((unsigned char*)inputStreamThread.mAcqs.front().mData, inputStream->getDims(), inputStream->getFormat());
            //            }
        }

        ++m_counterFpsThreads->m_counterFrame;
        emit newAcquisition(m_sensorName, sourceType);
    }
}

void FormInputStreamView::onStreamingStopped(const std::string& sourceType)
{
    std::cout << "[FormInputStreamView] onStreamingStopped(" << sourceType << ")" << std::endl;

    auto& inputStreamThreads = m_sourceType2inputStreamThreads.at(sourceType);

    assert(inputStreamThreads.size() >= 1);
    //    m_sourceType2inputStreamThreads[sourceType].push_back(std::make_unique<InputStreamThread>(std::make_unique<InputStream>(std::move(iostream)),
    //    sourceType));
    auto* inputStreamThread = inputStreamThreads.front().get();
    QObject::disconnect(inputStreamThread,
        &InputStreamThread::newAcquisition,
        this,
        &FormInputStreamView::onNewAcquisition);
    QObject::disconnect(inputStreamThread,
        &InputStreamThread::streamingStopped,
        this,
        &FormInputStreamView::onStreamingStopped);
    //    inputStreamThread->start();
    //    assert(! inputStreamThread->isRunning());
    if (inputStreamThread->isRunning()) {
        inputStreamThread->requestInterruption();
        inputStreamThread->wait();
    }

    //    inputStreamThreads.erase(inputStreamThreads.begin());
    inputStreamThreads.pop_front();

    if (inputStreamThreads.empty()) {
        std::cout << "[FormInputStreamView] onStreamingStopped(" << sourceType << ") no stream left"
                  << std::endl;
        auto stringList = m_sourceTypeModel.stringList();
        stringList.removeAll(sourceType.c_str());
        m_sourceTypeModel.setStringList(stringList);
        //    if (stringList.empty())
        //        ui->comboBox_sourceType.

        m_sourceType2inputStreamThreads.erase(sourceType);

        if (stringList.empty())
            emit init(m_sensorName);

        if (m_sourceType2inputStreamThreads.empty())
            emit isEmpty(m_sensorName);
    }
}
