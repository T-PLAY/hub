#include "FormInputStreamViews.h"
#include "ui_FormInputStreamViews.h"

#include <QComboBox>
#include <QLabel>
//#include <QHorizontalLayout>
#include <QHBoxLayout>

#include <RamIO.h>

#include <constants.h>

#include <Frame.h>

InputStreamThread::InputStreamThread(InputStream& inputStream, QObject* parent)
    : QThread(parent)
    , mInputStream(inputStream)
    , mSensorName(inputStream.getSensorName())
//    mInputStream( std::move(iostream))
{
    std::cout << "[InputStreamThread] InputStreamThread()" << std::endl;
}

InputStreamThread::~InputStreamThread()
{
    std::cout << "[InputStreamThread] ~InputStreamThread()" << std::endl;

    //    if (this->isRunning()) {
    //        this->requestInterruption();
    //        this->wait();
    //    }
}

void InputStreamThread::run()
{
    std::cout << "[InputStreamThread] run()" << std::endl;

    try {

        while (!this->isInterruptionRequested()) {

            mInputStream >> mAcq;
            //                        std::cout << "receive acq : " << mAcq << std::endl;

            emit newAcquisition(mSensorName);
        }
    } catch (std::exception& e) {
        std::cout << "[InputStreamThread] catch exception : " << e.what() << std::endl;
        emit streamingStopped(mSensorName);
        return;
    }

    std::cout << "[InputStreamThread] run() end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////

FormInputStreamViews::FormInputStreamViews(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormInputStreamViews)
    , m_recorder(PROJECT_DIR "data/")
{
    ui->setupUi(this);

    ui->comboBox_scan->setModel(&m_scanModel);
    ui->comboBox_pose->setModel(&m_poseModel);
}

FormInputStreamViews::~FormInputStreamViews()
{
    std::cout << "[FormInputStreamViews] ~FormInputStreamViews() start" << std::endl;
    //    for (const auto& pair : m_hBoxLayouts) {
    //        delete pair.second;
    //    }
    //    for (auto& pair : m_threads) {
    //        delete pair.second;
    //    }

    //    m_inputStreams.clear();
    //    m_threads.clear();

    delete ui;
    std::cout << "[FormInputStreamViews] ~FormInputStreamViews() end" << std::endl;
}

// void FormInputStreamViews::onComboBox_scan_currentTextChanged(const QString& sourceType)
//{
// }

// void FormInputStreamViews::onComboBox_pose_currentTextChanged(const QString& sourceType)
//{
// }

// void FormInputStreamViews::onNewScanAcquisition(const std::string& sensorName)
//{
//     //    std::cout << "[FormInputStreamViews] onNewScanAcquisition(" << sensorName << ")" << std::endl;

//    assert(sensorName == m_activeStreamScan);
//    //    if (m_threadInputStreamScan == nullptr) {
//    //        std::cout << "[on_newScanAcquisition] ##################################### nulptr"
//    //                  << std::endl;
//    //        m_comp->initScan();
//    //        return;
//    //    }

//    //    assert(m_threadInputStreamScan != nullptr);

//    //    m_comp->updateScan(m_threadInputStreamScan->mAcq);
//    emit updateScan();
//}

// void FormInputStreamViews::onNewPoseAcquisition(const std::string& sensorName)
//{
//         std::cout << "[FormInputStreamViews] onNewPoseAcquisition(" << sensorName << ")" << std::endl;

//    assert(sensorName == m_activeStreamPose);

//    emit updatePose();
//    //    if (m_threadInputStreamPose == nullptr) {
//    //        std::cout << "[on_newPoseAcquisition] ##################################### nulptr"
//    //                  << std::endl;
//    //        m_comp->initProbe();
//    //        return;
//    //    }

//    //    assert(m_threadInputStreamPose != nullptr);

//    //    m_comp->updateProbe(m_threadInputStreamPose->mAcq);
//}

void FormInputStreamViews::onNewInputStream(std::string streamName)
{

    //    QComboBox & comboBox = ui->comboBox_pose;

    auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
    if (sensorName == g_probeScanSensorName) {

        const auto sourceType = (sensorName.size() == streamName.size())
            ? ("physical")
            : (streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2));

        //        QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_scan_currentTextChanged);
        //        ui->comboBox_scan->addItem(sourceType.c_str());

        //        auto & stringList =
        //        m_scanModel.setStringList()
        auto stringList = m_scanModel.stringList();
        stringList.append(sourceType.c_str());
        m_scanModel.setStringList(stringList);

        //        if (m_scanModel.insertRow(m_scanModel.rowCount())) {
        //            auto index = m_scanModel.index(m_scanModel.rowCount() - 1, 0);
        //            m_scanModel.setData(index, sourceType.c_str());
        //        }
    }

    sensorName = streamName.substr(0, g_probePoseSensorName.size());
    if (sensorName == g_probePoseSensorName) {

        const auto sourceType = (sensorName.size() == streamName.size())
            ? ("physical")
            : (streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2));

        //        QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_pose_currentTextChanged);
        //        ui->comboBox_pose->addItem(sourceType.c_str());

        auto stringList = m_poseModel.stringList();
        stringList.append(sourceType.c_str());
        m_poseModel.setStringList(stringList);
        //        if (m_poseModel.insertRow(m_poseModel.rowCount())) {
        //            auto index = m_poseModel.index(m_poseModel.rowCount() - 1, 0);
        //            m_poseModel.setData(index, sourceType.c_str());
        //        }
    }
    //    }
}

void FormInputStreamViews::onDeleteInputStream(const std::string& streamName)
{
    std::cout << "[FormInputStreamViews] onDeleteInputStream(" << streamName << ")" << std::endl;

    auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
    if (sensorName == g_probeScanSensorName) {

        const auto sourceType = (sensorName.size() == streamName.size())
            ? ("physical")
            : (streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2));

        //        QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_scan_currentTextChanged);
        //        ui->comboBox_scan->addItem(sourceType.c_str());

        //        auto & stringList =
        //        m_scanModel.setStringList()
        auto stringList = m_scanModel.stringList();
        //        stringList.append(sourceType.c_str());
        //        stringList.remove(sourceType.c_str());
        stringList.removeAll(sourceType.c_str());
        m_scanModel.setStringList(stringList);

        // empty model do not imply the text view change
        if (stringList.empty())
            on_comboBox_scan_currentTextChanged("");

        //        if (m_scanModel.insertRow(m_scanModel.rowCount())) {
        //            auto index = m_scanModel.index(m_scanModel.rowCount() - 1, 0);
        //            m_scanModel.setData(index, sourceType.c_str());
        //        }
    }

    sensorName = streamName.substr(0, g_probePoseSensorName.size());
    if (sensorName == g_probePoseSensorName) {

        const auto sourceType = (sensorName.size() == streamName.size())
            ? ("physical")
            : (streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2));

        //        QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_pose_currentTextChanged);
        //        ui->comboBox_pose->addItem(sourceType.c_str());

        auto stringList = m_poseModel.stringList();
        //        stringList.append(sourceType.c_str());
        stringList.removeAll(sourceType.c_str());
        m_poseModel.setStringList(stringList);

        // empty model do not imply the text view change
        if (stringList.empty())
            on_comboBox_pose_currentTextChanged("");

        //        if (m_poseModel.insertRow(m_poseModel.rowCount())) {
        //            auto index = m_poseModel.index(m_poseModel.rowCount() - 1, 0);
        //            m_poseModel.setData(index, sourceType.c_str());
        //        }
    }
    //    }

    assert(m_threads.find(streamName) != m_threads.end());
    m_threads.erase(streamName);

    assert(m_inputStreams.find(streamName) != m_inputStreams.end());
    m_inputStreams.erase(streamName);
}

// void FormInputStreamViews::onNewInputStream(const IOStream &&iostream)
//{

//}

// void FormInputStreamViews::on_startStreaming(std::string streamName)
//{
//     //    assert(m_sensorViews != nullptr);

//    //    const auto & sensorView = m_sensorViews->getSensorView(streamName);
//    //    const auto & inputStream = sensorView.getInputStream();

////        auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
////        if (sensorName == g_probeScanSensorName) {

////            const auto sourceType = (sensorName.size() == streamName.size())
////                ? ("physical")
////                : (streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2));

////            QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_scan_currentTextChanged);
////            comboBox->addItem(sourceType.c_str());
////        }

////        sensorName = streamName.substr(0, g_probePoseSensorName.size());
////        if (sensorName == g_probePoseSensorName) {

////            const auto sourceType = (sensorName.size() == streamName.size())
////                ? ("physical")
////                : (streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2));

////            QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_pose_currentTextChanged);
////            comboBox->addItem(sourceType.c_str());
////        }
//}

// void FormInputStreamViews::on_stopStreaming(std::string streamName)
//{
//     std::cout << "[FormInputStreamViews] on_stopStreaming " << streamName << std::endl;

//    //    assert(m_sensorViews != nullptr);

//    //    if (streamName.substr(0, g_probeScanSensorName.size()) == g_probeScanSensorName) {
//    //        auto sourceType = (g_probeScanSensorName.size() == streamName.size())
//    //            ? ("physical")
//    //            : (streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2));
//    //        int iItem = ui->comboBox_scan->findText(sourceType.c_str());
//    //        assert(iItem >= 0);
//    //        ui->comboBox_scan->removeItem(iItem);

//    //    } else if (streamName.substr(0, g_probePoseSensorName.size()) == g_probePoseSensorName) {
//    //        auto sourceType = (g_probePoseSensorName.size() == streamName.size())
//    //            ? ("physical")
//    //            : (streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2));
//    //        int iItem = ui->comboBox_pose->findText(sourceType.c_str());
//    //        assert(iItem >= 0);
//    //        ui->comboBox_pose->removeItem(iItem);
//    //    }
//}

const Stream::Acquisition& FormInputStreamViews::getScanAcquisition() const
{
    //    return m_thre
    assert(m_threads.find(m_activeStreamScan) != m_threads.end());
    return m_threads.at(m_activeStreamScan)->mAcq;
}

const Stream::Acquisition& FormInputStreamViews::getPoseAcquisition() const
{
    assert(m_threads.find(m_activeStreamPose) != m_threads.end());
    return m_threads.at(m_activeStreamPose)->mAcq;
}

// void FormInputStreamViews::onNewAcquisition(const std::string &sensorName)
//{
//     std::cout << "[FormInputStreamViews] onNewAcquisition(" << sensorName << ")" << std::endl;

//}

// void FormInputStreamViews::connect(const std::map<std::string, std::unique_ptr<CyclicBuff>>& ramOutputStreams)
//{

//    for (const auto& pair : ramOutputStreams) {
//        const auto& streamName = pair.first;
//        auto& cyclicBuff = const_cast<CyclicBuff&>(*pair.second);

//        //        IOStream & ioStream = cyclicBuff.getIOStream();
//        //        RamIO & ramIO = dynamic_cast<RamIO&>(ioStream);
//        //        InputStream inputStream(ramIO);
//        std::cout << "connected " << streamName << std::endl;

//        addInputStream(streamName, RamIO(cyclicBuff));

////        QLabel* label = new QLabel(streamName.c_str());
////        QComboBox* comboBox = new QComboBox();
////        QHBoxLayout* layout = new QHBoxLayout();
////        layout->addWidget(label);
////        layout->addWidget(comboBox);
////        m_hBoxLayouts[streamName] = layout;

//        //        ui->horizontalLayout->addLayout(layout);
//        //        ui->horizontalLayout->insertLayout(1, layout);
////        ui->horizontalLayout_inputStreams->addLayout(layout);

////        auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
////        if (sensorName == g_probeScanSensorName) {

////            const auto sourceType = (sensorName.size() == streamName.size())
////                ? ("physical")
////                : (streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2));

////            QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_scan_currentTextChanged);
////            comboBox->addItem(sourceType.c_str());
////        }

////        sensorName = streamName.substr(0, g_probePoseSensorName.size());
////        if (sensorName == g_probePoseSensorName) {

////            const auto sourceType = (sensorName.size() == streamName.size())
////                ? ("physical")
////                : (streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2));

////            QObject::connect(comboBox, &QComboBox::currentTextChanged, this, &FormInputStreamViews::onComboBox_pose_currentTextChanged);
////            comboBox->addItem(sourceType.c_str());
////        }
//    }
//}

void FormInputStreamViews::on_comboBox_scan_currentTextChanged(const QString& sourceType)
{
    std::cout << "[FormInputStreamViews] on_comboBox_scan_currentTextChanged(" << sourceType.toStdString() << ")" << std::endl;

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
            &FormInputStreamViews::newAcquisitionScan);
    }

    if (sourceType == "") {
        assert(m_activeStreamScan != "");
        m_threadInputStreamScan = nullptr;

        emit initScan();
        m_activeStreamScan = "";
        return;
        //            m_comp->initScan();
    } else {
        //            const auto& sensorView = m_sensorViews->getSensorView(streamName);
        m_threadInputStreamScan = m_threads.at(streamName).get();

        QObject::connect(m_threadInputStreamScan,
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::newAcquisitionScan);
    }
    m_activeStreamScan = streamName;
}

void FormInputStreamViews::on_comboBox_pose_currentTextChanged(const QString& sourceType)
{
    std::cout << "[FormInputStreamViews] on_comboBox_pose_currentTextChanged(" << sourceType.toStdString() << ")" << std::endl;

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
            &FormInputStreamViews::newAcquisitionPose);
    }

    //        QObject::connect(inputStreamThread, &InputStreamThread::newAcquisition, this, &FormInputStreamViews::onNewAcquisition);

    if (sourceType == "") {
        assert(m_activeStreamPose != "");
        m_threadInputStreamPose = nullptr;
        //            m_comp->initProbe();

        emit initPose();
        m_activeStreamPose = "";
        return;

    } else {
        //            const auto& sensorView = m_sensorViews->getSensorView(streamName);
        //            m_threadInputStreamPose = sensorView.getInputStreamThread();

        assert(m_threads.find(streamName) != m_threads.end());
        m_threadInputStreamPose = m_threads.at(streamName).get();

        QObject::connect(m_threadInputStreamPose,
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::newAcquisitionPose);
    }

    m_activeStreamPose = streamName;
}

void FormInputStreamViews::on_comboBox_scan_currentIndexChanged(int index)
{
    std::cout << "[FormInputStreamViews] on_comboBox_scan_currentIndexChanged(" << index << ")" << std::endl;
}

void FormInputStreamViews::on_comboBox_pose_currentIndexChanged(int index)
{
    std::cout << "[FormInputStreamViews] on_comboBox_pose_currentIndexChanged(" << index << ")" << std::endl;
}

void FormInputStreamViews::on_toolButton_record_clicked()
{
    if (m_recorder.isRecording()) {

        m_recorder.stop();
        ui->toolButton_record->setText("startRecording");

    } else {
        InputStreamParameters inputStreamParameters;
        if (m_activeStreamScan != "") {
            inputStreamParameters.push_back({ m_activeStreamScan, "" });
        }
        if (m_activeStreamPose != "") {
            inputStreamParameters.push_back({ m_activeStreamPose, "" });
        }
        if (inputStreamParameters.empty()) {
            return;
        }
        m_recorder.record(inputStreamParameters);
        ui->toolButton_record->setText("stopRecording");
    }
}

void FormInputStreamViews::on_toolButton_snapshot_clicked()
{
    Frame frame;

    if (m_activeStreamScan != "") {
        Snapshot scanSnapshot(m_threadInputStreamScan->mInputStream, m_threadInputStreamScan->mAcq);
        frame.push_back(scanSnapshot);
    }
    if (m_activeStreamPose != "") {
        Snapshot poseSnapshot(m_threadInputStreamPose->mInputStream, m_threadInputStreamPose->mAcq);
        frame.push_back(poseSnapshot);
    }
    if (frame.empty()) {
        return;
    }
    m_recorder.save(frame);
}
