#include "FormInputStreamViews.h"
#include "ui_FormInputStreamViews.h"

#include <QComboBox>
#include <QLabel>
//#include <QHorizontalLayout>
#include <QHBoxLayout>

#include <RamIO.h>

#include <constants.h>

InputStreamThread::InputStreamThread(InputStream& inputStream, QObject* parent)
    : QThread(parent)
    , mInputStream(inputStream)
    , mSensorName(inputStream.getSensorName())
//    mInputStream( std::move(iostream))
{
    std::cout << "InputStreamThread()" << std::endl;
}

InputStreamThread::~InputStreamThread()
{
    std::cout << "~InputStreamThread()" << std::endl;
}

void InputStreamThread::run()
{
    std::cout << "InputStreamThread::run()" << std::endl;

    try {

        while (!this->isInterruptionRequested()) {

            mInputStream >> mAcq;
//            std::cout << "receive acq : " << mAcq << std::endl;

            emit newAcquisition(mSensorName);
        }
    } catch (std::exception& e) {
        std::cout << "[streamView] catch exception : " << e.what() << std::endl;
        //        emit streamingStopped(mInputStream.getSensorName());
        return;
    }

    std::cout << "InputStreamThread::run() end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////

FormInputStreamViews::FormInputStreamViews(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormInputStreamViews)
{
    ui->setupUi(this);
}

FormInputStreamViews::~FormInputStreamViews()
{
    std::cout << "[FormInputStreamViews] ~FormInputStreamViews() start" << std::endl;
    for (const auto& pair : m_hBoxLayouts) {
        delete pair.second;
    }
    for (const auto& pair : m_threads) {
        delete pair.second;
    }
    delete ui;
    std::cout << "[FormInputStreamViews] ~FormInputStreamViews() end" << std::endl;
}

void FormInputStreamViews::onComboBox_scan_currentTextChanged(const QString& sourceType)
{
    const auto& streamName = (sourceType == "physical")
        ? (g_probeScanSensorName)
        : (g_probeScanSensorName + " (" + sourceType.toStdString() + ")");

    std::cout << "[on_comboBox_scan_currentTextChanged] " << streamName << std::endl;
    //    assert(m_sensorViews != nullptr);

    if (sourceType != "" && m_activeStreamScan != "" && streamName != m_activeStreamScan) {
        assert(m_threadInputStreamScan != nullptr);
        QObject::disconnect(m_threadInputStreamScan,
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::onNewScanAcquisition);
    }

    if (sourceType == "") {
        assert(m_activeStreamScan != "");
        m_threadInputStreamScan = nullptr;

        emit initScan();
        //            m_comp->initScan();
    } else {
        //            const auto& sensorView = m_sensorViews->getSensorView(streamName);
        m_threadInputStreamScan = m_threads.at(streamName);

        QObject::connect(m_threadInputStreamScan,
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::onNewScanAcquisition);
    }
    m_activeStreamScan = streamName;
}

void FormInputStreamViews::onComboBox_pose_currentTextChanged(const QString& sourceType)
{
    const auto& streamName = (sourceType == "physical")
        ? (g_probePoseSensorName)
        : (g_probePoseSensorName + " (" + sourceType.toStdString() + ")");

    std::cout << "[on_comboBox_pose_currentTextChanged] " << streamName << std::endl;
    //    assert(m_sensorViews != nullptr);

    if (sourceType != "" && m_activeStreamPose != "" && streamName != m_activeStreamPose) {
        assert(m_threadInputStreamPose != nullptr);
        QObject::disconnect(m_threadInputStreamPose,
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::onNewPoseAcquisition);
    }

    //        QObject::connect(inputStreamThread, &InputStreamThread::newAcquisition, this, &FormInputStreamViews::onNewAcquisition);

    if (sourceType == "") {
        assert(m_activeStreamPose != "");
        m_threadInputStreamPose = nullptr;
        //            m_comp->initProbe();

        emit initPose();

    } else {
        //            const auto& sensorView = m_sensorViews->getSensorView(streamName);
        //            m_threadInputStreamPose = sensorView.getInputStreamThread();

        assert(m_threads.find(streamName) != m_threads.end());
        m_threadInputStreamPose = m_threads.at(streamName);

        QObject::connect(m_threadInputStreamPose,
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::onNewPoseAcquisition);
    }

    m_activeStreamPose = streamName;
}

void FormInputStreamViews::onNewScanAcquisition(const std::string& sensorName)
{
//    std::cout << "[FormInputStreamViews] onNewScanAcquisition(" << sensorName << ")" << std::endl;

    assert(sensorName == m_activeStreamScan);
    //    if (m_threadInputStreamScan == nullptr) {
    //        std::cout << "[on_newScanAcquisition] ##################################### nulptr"
    //                  << std::endl;
    //        m_comp->initScan();
    //        return;
    //    }

    //    assert(m_threadInputStreamScan != nullptr);

    //    m_comp->updateScan(m_threadInputStreamScan->mAcq);
    emit updateScan();
}

void FormInputStreamViews::onNewPoseAcquisition(const std::string& sensorName)
{
//    std::cout << "[FormInputStreamViews] onNewPoseAcquisition(" << sensorName << ")" << std::endl;

    assert(sensorName == m_activeStreamPose);

    emit updatePose();
    //    if (m_threadInputStreamPose == nullptr) {
    //        std::cout << "[on_newPoseAcquisition] ##################################### nulptr"
    //                  << std::endl;
    //        m_comp->initProbe();
    //        return;
    //    }

    //    assert(m_threadInputStreamPose != nullptr);

    //    m_comp->updateProbe(m_threadInputStreamPose->mAcq);
}

const Stream::Acquisition &FormInputStreamViews::getScanAcquisition() const
{
//    return m_thre
    assert(m_threads.find(m_activeStreamScan) != m_threads.end());
    return m_threads.at(m_activeStreamScan)->mAcq;

}

const Stream::Acquisition &FormInputStreamViews::getPoseAcquisition() const
{
    assert(m_threads.find(m_activeStreamPose) != m_threads.end());
    return m_threads.at(m_activeStreamPose)->mAcq;
}

// void FormInputStreamViews::onNewAcquisition(const std::string &sensorName)
//{
//     std::cout << "[FormInputStreamViews] onNewAcquisition(" << sensorName << ")" << std::endl;

//}

void FormInputStreamViews::connect(const std::map<std::string, CyclicBuff>& ramOutputStreams)
{

    for (const auto& pair : ramOutputStreams) {
        const auto& streamName = pair.first;
        auto& cyclicBuff = const_cast<CyclicBuff&>(pair.second);

        //        IOStream & ioStream = cyclicBuff.getIOStream();
        //        RamIO & ramIO = dynamic_cast<RamIO&>(ioStream);
        //        InputStream inputStream(ramIO);

        assert(m_inputStreams.find(streamName) == m_inputStreams.end());
        m_inputStreams[streamName] = std::make_unique<InputStream>(RamIO(cyclicBuff));

        //        m_threads[streamName] = std::make_unique<InputStreamThread>(InputStreamThread(*m_inputStreams[streamName]));
        auto* inputStreamThread = new InputStreamThread(*m_inputStreams.at(streamName));
        //        QObject::connect(inputStreamThread, &InputStreamThread::newAcquisition, this, &FormInputStreamViews::onNewAcquisition);
        assert(m_threads.find(streamName) == m_threads.end());
        m_threads[streamName] = inputStreamThread;
        inputStreamThread->start();

        //        m_threads[streamName] = InputStreamThread(RamIO(cyclicBuff));
        //        InputStream()

        QLabel* label = new QLabel(streamName.c_str());
        QComboBox* comboBox = new QComboBox();
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget(label);
        layout->addWidget(comboBox);
        m_hBoxLayouts[streamName] = layout;

        //        ui->horizontalLayout->addLayout(layout);
        //        ui->horizontalLayout->insertLayout(1, layout);
        ui->horizontalLayout_inputStreams->addLayout(layout);

        std::cout << "connected " << streamName << std::endl;

        auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
        if (sensorName == g_probeScanSensorName) {

            const auto sourceType = (sensorName.size() == streamName.size())
                ? ("physical")
                : (streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2));

            QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_scan_currentTextChanged);
            comboBox->addItem(sourceType.c_str());
        }

        sensorName = streamName.substr(0, g_probePoseSensorName.size());
        if (sensorName == g_probePoseSensorName) {

            const auto sourceType = (sensorName.size() == streamName.size())
                ? ("physical")
                : (streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2));

            QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_pose_currentTextChanged);
            comboBox->addItem(sourceType.c_str());
        }
    }
}
