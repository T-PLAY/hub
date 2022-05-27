#include "FormInputStreamViews.h"
#include "ui_FormInputStreamViews.h"

#include <QComboBox>
#include <QLabel>
//#include <QHorizontalLayout>
#include <QHBoxLayout>

#include <RamIO.h>

#include <constants.h>

#include <Frame.h>

FormInputStreamViews::FormInputStreamViews(QMdiArea &mdiArea, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormInputStreamViews)
    , m_recorder(PROJECT_DIR "data/")
    , m_mdiArea(mdiArea)
//    , m_counterFpsThreads[1](*ui->label_poseFrequency)
//    , m_counterFpsThreads[0](*ui->label_scanFrequency)
{
    ui->setupUi(this);

    m_streamViewsLayout = ui->horizontalLayout_streamView;

    //    QObject::connect(this, &FormInputStreamViews::newAcquisitionPose, this,
    //    &FormInputStreamViews::onNewAcquisitionPose); QObject::connect(this,
    //    &FormInputStreamViews::newAcquisitionScan, this,
    //    &FormInputStreamViews::onNewAcquisitionScan);

    //    QObject::connect(this,
    //        &FormInputStreamViews::newAcquisition,
    //        this,
    //        &FormInputStreamViews::onNewAcquisition);

    //    QObject::connect(this,
    //        &FormInputStreamViews::newAcquisitionScan,
    //        this,
    //        &FormInputStreamViews::onNewAcquisitionScan);

    //    m_counterFpsThreads[1].start();
    //    m_counterFpsThreads[0].start();
    setEnabled(false);
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

void FormInputStreamViews::deleteInputStream(const std::string& streamName)
{

//    std::string sensorName;
//    int iSensor = -1;
//    bool found = false;
//    while (!found && iSensor < 2) {
//        ++iSensor;

//        sensorName = m_sensorNames[iSensor];
//        found = streamName.substr(0, sensorName.size()) == sensorName;
//    }
//    assert(found);

    std::string sensorName = "";
    for (std::string sourceType : { "record" }) {
        const int start = streamName.size() - sourceType.size() - 1;
        if (start < 0)
            continue;
        const std::string subStr = streamName.substr(start, sourceType.size());
        if (subStr == sourceType) {
            sensorName = streamName.substr(0, start - 2);
            break;
        }
    }

    if (sensorName.empty())
        sensorName = streamName;

    assert (m_sensorName2streamView.find(sensorName) != m_sensorName2streamView.end());

    //    assert(m_sensorName2streamView.find(sensorName) != m_sensorName2streamView.end());
    if (m_sensorName2streamView.find(sensorName) != m_sensorName2streamView.end()) {
        FormInputStreamView* inputStreamView = m_sensorName2streamView.at(sensorName);

        const auto sourceType = (sensorName.size() == streamName.size())
            ? ("physical")
            : (streamName.substr(sensorName.size() + 2,
                streamName.size() - 1 - sensorName.size() - 2));
        assert(sourceType == "physical" || sourceType == "record");

        inputStreamView->remove(sourceType);
    }
}

Acquisitions & FormInputStreamViews::getAcquisitions(const std::string &sensorName, const std::string &sourceType)
{
    assert(m_sensorName2streamView.find(sensorName) != m_sensorName2streamView.end());
    return m_sensorName2streamView.at(sensorName)->getAcquisitions(sourceType);
}

//const Stream::Acquisition& FormInputStreamViews::getAcquisition(const std::string& sensorName, const std::string& sourceType) const
//{
//    assert(m_sensorName2streamView.find(sensorName) != m_sensorName2streamView.end());
//    return m_sensorName2streamView.at(sensorName)->getAcquisition(sourceType);
//}

void FormInputStreamViews::onNewAcquisition(const std::string& sensorName, const std::string& sourceType)
{
    if (m_recorder.isRecording()) {
        //    m_recorder.savePose(getPoseAcquisition());
        const auto& inputStreamThread = m_sensorName2streamView.at(sensorName)->getIputStreamThread(sourceType);
        //        m_recorder.add(Snapshot(*inputStreamThread.mInputStream, inputStreamThread.mAcq));
        m_recorder.add(
            Snapshot(sensorName,
                inputStreamThread.mInputStream->getFormat(),
                inputStreamThread.mInputStream->getDims(),
                inputStreamThread.mAcqs.back()));

        //            m_recorder.save(inputStreamThread.mInputStream);
    }
}

void FormInputStreamViews::onDeleteStreamView(const std::string &sensorName)
{
    auto * formInputStreamView = m_sensorName2streamView.at(sensorName);
    m_sensorName2streamView.erase(sensorName);
    m_streamViewsLayout->removeWidget(formInputStreamView);
    delete formInputStreamView;

    if (m_sensorName2streamView.empty())
        setEnabled(false);
}

// void FormInputStreamViews::onKillInputStream(const std::string& streamName)
//{

//    int iSensor = -1;
//    bool found = false;
//    while (!found && iSensor < 2) {
//        ++iSensor;

//        auto sensorName = m_sensorNames[iSensor];
//        found = streamName.substr(0, sensorName.size()) == sensorName;
//    }
//    assert(found);

//    onKillInputStreamPrivate(streamName, iSensor);
//}

// void FormInputStreamViews::onKillInputStreamPrivate(const std::string& streamName, int iSensor)
//{
//     std::cout << "[FormInputStreamViews] onKillInputStreamPrivate(" << streamName << ")" << std::endl;

//    auto& streamsKilled = m_streamsKilled[iSensor];

//    if (streamsKilled.find(streamName) != streamsKilled.end()) {
//        streamsKilled.erase(streamName);
//        return;
//    }
//    onDeleteInputStream(streamName, iSensor);

//    //    assert(m_threads.find(streamName) != m_threads.end());
//    //    auto& inputStreamThread = *m_threads.at(streamName);
//    //    assert(inputStreamThread.isRunning());
//    //    inputStreamThread.requestInterruption();
//    //    inputStreamThread.wait();
//}

// void FormInputStreamViews::onNewAcquisition(const std::string& streamName, int iSensor)
//{
////    //    assert(m_activeThreadInputStreams[1] != nullptr);
////    if (m_activeThreadInputStreams[iSensor] == nullptr)
////        return;

////    assert(m_activeStreamNames[iSensor] != "");
////    assert(streamName == m_activeStreamNames[iSensor]);

////    //    assert( m_recorder.isRecording() );
////    if (m_recorder.isRecording()) {
////        //    m_recorder.savePose(getPoseAcquisition());
////        m_recorder.add(
////            //        Snapshot( m_activeThreadInputStreams[iSensor]->mInputStream, m_activeThreadInputStreams[iSensor]->mAcq )
////            //        );
////            Snapshot(m_sensorNames[iSensor],
////                m_activeThreadInputStreams[iSensor]->mInputStream.getFormat(),
////                m_activeThreadInputStreams[iSensor]->mInputStream.getDims(),
////                m_activeThreadInputStreams[iSensor]->mAcq));
////        //    m_recorder.save(m_activeStreamNames[iSensor], m_activeThreadInputStreams[iSensor]->mAcq);
////    }

////    m_lastAcqs[iSensor] = m_activeThreadInputStreams[iSensor]->mAcq.clone();

////    //    ++mCounterFramePose;
////    ++m_counterFpsThreads[iSensor]->m_counterFrame;

////    if (iSensor == 0) {
////        emit newAcquisitionScan();
////    } else if (iSensor == 1) {
////        emit newAcquisitionPose();
////    }
//}

// void FormInputStreamViews::onNewInputStream(const std::string& streamName, int iSensor)
//{

////    //    QComboBox & comboBox = ui->comboBox_pose;

////    const auto sensorName = m_sensorNames[iSensor];

////    assert(streamName.substr(0, sensorName.size()) == sensorName);

////    //    auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
////    //    if (sensorName == g_probeScanSensorName) {

////    const auto sourceType = (sensorName.size() == streamName.size())
////        ? ("physical")
////        : (streamName.substr(sensorName.size() + 2,
////            streamName.size() - 1 - sensorName.size() - 2));

////    assert(sourceType == "" || sourceType == "physical" || sourceType == "record");

////    //        QObject::connect(comboBox, &QComboBox::currentTextChanged, this,
////    //        &FormInputStreamViews::onComboBox_scan_currentTextChanged);
////    //        ui->comboBox_scan->addItem(sourceType.c_str());

////    //        auto & stringList =
////    //        m_streamModels[0].setStringList()

////    auto stringList = m_streamModels[iSensor].stringList();
////    for (const auto& string : stringList) {
////        if (string == sourceType.c_str()) {
////            return;
////        }
////    }
////    stringList.append(sourceType.c_str());
////    m_streamModels[iSensor].setStringList(stringList);

////    //        if (m_streamModels[0].insertRow(m_streamModels[0].rowCount())) {
////    //            auto index = m_streamModels[0].index(m_streamModels[0].rowCount() - 1, 0);
////    //            m_streamModels[0].setData(index, sourceType.c_str());
////    //        }
////    //    }
//}

// void FormInputStreamViews::onDeleteInputStream(const std::string& streamName)
//{
////    std::cout << "[FormInputStreamViews] onDeleteInputStream(" << streamName << ")" << std::endl;

////    auto& threads = m_threads[iSensor];

////    //    auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
////    auto sensorName = m_sensorNames[iSensor];
////    //    if (sensorName == g_probeScanSensorName) {

////    QObject::disconnect(threads.at(streamName).get(),
////        &InputStreamThread::newAcquisition,
////        this,
////        &FormInputStreamViews::newAcquisition);

////    const auto sourceType = (sensorName.size() == streamName.size())
////        ? ("physical")
////        : (streamName.substr(sensorName.size() + 2,
////            streamName.size() - 1 - sensorName.size() - 2));

////    assert(sourceType == "" || sourceType == "physical" || sourceType == "record");
////    //        QObject::connect(comboBox, &QComboBox::currentTextChanged, this,
////    //        &FormInputStreamViews::onComboBox_scan_currentTextChanged);
////    //        ui->comboBox_scan->addItem(sourceType.c_str());

////    //        auto & stringList =
////    //        m_streamModels[0].setStringList()
////    auto stringList = m_streamModels[iSensor].stringList();
////    //        stringList.append(sourceType.c_str());
////    //        stringList.remove(sourceType.c_str());
////    stringList.removeAll(sourceType.c_str());
////    m_streamModels[iSensor].setStringList(stringList);

////    // empty model do not imply the text view change
////    if (stringList.empty())
////        onComboBox_currentTextChanged("", iSensor);

////    //        if (m_streamModels[0].insertRow(m_streamModels[0].rowCount())) {
////    //            auto index = m_streamModels[0].index(m_streamModels[0].rowCount() - 1, 0);
////    //            m_streamModels[0].setData(index, sourceType.c_str());
////    //        }
////    //    }

////    assert(threads.find(streamName) != threads.end());
////    threads.erase(streamName);

////    auto& inputStreams = m_inputStreams[iSensor];

////    assert(inputStreams.find(streamName) != inputStreams.end());
////    inputStreams.erase(streamName);
//}

// const Stream::Acquisition& FormInputStreamViews::getScanAcquisition() const
//{
//     //    return m_thre
//     //    if (m_threads.find(m_activeStreamNames[0]) == m_threads.end())
//     //        return ;

//    //    assert(m_threads.find(m_activeStreamNames[0]) != m_threads.end());
//    //    return m_threads.at(m_activeStreamNames[0])->mAcq;

//    return m_lastAcqs[0];
//}

// const Stream::Acquisition& FormInputStreamViews::getPoseAcquisition() const
//{
//     //    assert(m_threads.find(m_activeStreamNames[1]) != m_threads.end());
//     //    return m_threads.at(m_activeStreamNames[1])->mAcq;

//    return m_lastAcqs[1];
//}

// void FormInputStreamViews::onComboBox_currentTextChanged(const QString& sourceType, int iSensor)
//{
////    std::cout << "[FormInputStreamViews] on_comboBox_currentTextChanged('"
////              << sourceType.toStdString() << "', " << iSensor << ")" << std::endl;

////    assert(sourceType == "" || sourceType == "physical" || sourceType == "record");

////    if (sourceType == "") {
////        assert(m_activeStreamNames[iSensor] != "");
////        QObject::disconnect(m_activeThreadInputStreams[iSensor],
////            &InputStreamThread::newAcquisition,
////            this,
////            &FormInputStreamViews::newAcquisition);
////        m_activeThreadInputStreams[iSensor] = nullptr;

////        if (iSensor == 0)
////            emit initScan();
////        else
////            emit initPose();
////        m_activeStreamNames[iSensor] = "";
////        return;
////    }

////    //            m_comp->initScan();
////    const auto& streamName = (sourceType == "physical")
////        ? (m_sensorNames[iSensor])
////        : (m_sensorNames[iSensor] + " (" + sourceType.toStdString() + ")");

////    if (m_activeStreamNames[iSensor] == streamName)
////        return;

////    //    std::cout << "[onComboBox_currentTextChanged] '" << streamName << "'" << ", iSensor = " << iSensor << std::endl;
////    //    assert(m_sensorViews != nullptr);

////    if (m_activeStreamNames[iSensor] != "" && streamName != m_activeStreamNames[iSensor]) {
////        assert(m_activeThreadInputStreams[iSensor] != nullptr);
////        QObject::disconnect(m_activeThreadInputStreams[iSensor],
////            &InputStreamThread::newAcquisition,
////            this,
////            &FormInputStreamViews::newAcquisition);
////    }

////    auto& threads = m_threads[iSensor];
////    assert(threads.find(streamName) != threads.end());
////    //    } else {
////    //            const auto& sensorView = m_sensorViews->getSensorView(streamName);
////    m_activeThreadInputStreams[iSensor] = threads.at(streamName).get();

////    QObject::connect(m_activeThreadInputStreams[iSensor],
////        &InputStreamThread::newAcquisition,
////        this,
////        &FormInputStreamViews::newAcquisition);
////    //    }
////    m_activeStreamNames[iSensor] = streamName;
//}

// void FormInputStreamViews::on_comboBox_scan_currentTextChanged(const QString& sourceType)
//{
//     std::cout << "[FormInputStreamViews] on_comboBox_scan_currentTextChanged('"
//               << sourceType.toStdString() << "')" << std::endl;

//    onComboBox_currentTextChanged(sourceType, 0);
//}

// void FormInputStreamViews::on_comboBox_pose_currentTextChanged(const QString& sourceType)
//{
//     std::cout << "[FormInputStreamViews] on_comboBox_pose_currentTextChanged('"
//               << sourceType.toStdString() << "')" << std::endl;

//    onComboBox_currentTextChanged(sourceType, 1);
//}

void FormInputStreamViews::on_toolButton_record_clicked()
{
    if (m_recorder.isRecording()) {
        //    if (m_isRecording) {
        //        QObject::disconnect( this,
        //                             &FormInputStreamViews::newAcquisitionPose,
        //                             this,
        //                             &FormInputStreamViews::onNewAcquisitionPose );
        //        QObject::disconnect( this,
        //                             &FormInputStreamViews::newAcquisitionScan,
        //                             this,
        //                             &FormInputStreamViews::onNewAcquisitionScan );

        m_recorder.stop();
        //        m_recorder.saveOnDisk();
        ui->toolButton_record->setText("startRecording");
        ui->toolButton_record->setStyleSheet("background-color: none");
    } else {
        //        InputStreamParameters inputStreamParameters;
        //        if (m_activeStreamNames[0] != "") {
        //            inputStreamParameters.push_back({ m_activeStreamNames[0], "" });
        //        }
        //        if (m_activeStreamNames[1] != "") {
        //            inputStreamParameters.push_back({ m_activeStreamNames[1], "" });
        //        }
        //        if (inputStreamParameters.empty()) {
        //            return;
        //        }
        if (m_sensorName2streamView.empty())
            return;

        m_recorder.record();
        ui->toolButton_record->setText("stopRecording");
        //        QObject::connect( this,
        //                          &FormInputStreamViews::newAcquisitionPose,
        //                          this,
        //                          &FormInputStreamViews::onNewAcquisitionPose );
        //        QObject::connect( this,
        //                          &FormInputStreamViews::newAcquisitionScan,
        //                          this,
        //                          &FormInputStreamViews::onNewAcquisitionScan );
        ui->toolButton_record->setStyleSheet("background-color: red");
    }
}

// void FormInputStreamViews::on_toolButton_snapshot_clicked()
//{
//     assert(!m_recorder.isRecording());
//     Frame frame;

//    if (m_activeStreamNames[0] != "") {
//        Snapshot scanSnapshot(m_activeThreadInputStreams[0]->mInputStream,
//            m_activeThreadInputStreams[0]->mAcq);
//        frame.push_back(scanSnapshot);
//    }
//    if (m_activeStreamNames[1] != "") {
//        Snapshot poseSnapshot(m_activeThreadInputStreams[1]->mInputStream,
//            m_activeThreadInputStreams[1]->mAcq);
//        frame.push_back(poseSnapshot);
//    }
//    if (frame.empty()) {
//        return;
//    }
//    m_recorder.save(frame);
//}
