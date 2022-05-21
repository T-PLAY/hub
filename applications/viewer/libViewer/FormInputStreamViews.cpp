#include "FormInputStreamViews.h"
#include "ui_FormInputStreamViews.h"

#include <QComboBox>
#include <QLabel>
//#include <QHorizontalLayout>
#include <QHBoxLayout>

#include <RamIO.h>

#include <constants.h>

#include <Frame.h>

InputStreamThread::InputStreamThread(InputStream& inputStream,
    FormInputStreamViews& formInputStreamViews,
    int iSensor,
    QObject* parent)
    : QThread(parent)
    , mIStream(iSensor)
    , mInputStream(inputStream)
    , mSensorName(inputStream.getSensorName())
    , mFormInputStreamViews(formInputStreamViews)
//    mInputStream( std::move(iostream))
{
    std::cout << "[InputStreamThread] InputStreamThread()" << std::endl;
}

InputStreamThread::~InputStreamThread()
{
    std::cout << "[InputStreamThread] ~InputStreamThread()" << std::endl;

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

            mInputStream >> mAcq;
            //                        std::cout << "receive acq : " << mAcq << std::endl;

            emit newAcquisition(mSensorName, mIStream);
        }
    } catch (std::exception& e) {
        std::cout << "[InputStreamThread] catch exception : " << e.what() << std::endl;
        //        std::cout << "[InputStreamThread] streamingStopped : " << mSensorName <<
        //        std::endl;
        //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        emit streamingStopped(mSensorName, mIStream);
        //        mFormInputStreamViews.onKillInputStreamPrivate(mSensorName);
        return;
    }

    std::cout << "[InputStreamThread] run() end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////

CounterFpsThread::CounterFpsThread(QLabel& label, const std::string& sensorName)
    : m_sensorName(sensorName)
    , m_labelFps(label)
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

        m_labelFps.setText((std::to_string(1000.0 * m_counterFrame / duration) + " Hz").c_str());
        m_counterFrame = 0;

        //        std::cout << "[CounterFpsThread:" << m_sensorName << "] update" << std::endl;
        //        double fps = m_counterFrame * 10.0;
    }
    //    assert(false);
}

///////////////////////////////////////////////////////////////////////////////////

FormInputStreamViews::FormInputStreamViews(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormInputStreamViews)
    , m_recorder(PROJECT_DIR "data/")
//    , m_counterFpsThreads[1](*ui->label_poseFrequency)
//    , m_counterFpsThreads[0](*ui->label_scanFrequency)
{
    ui->setupUi(this);

    ui->comboBox_scan->setModel(&m_streamModels[0]);
    ui->comboBox_pose->setModel(&m_streamModels[1]);

    //    QObject::connect(this, &FormInputStreamViews::newAcquisitionPose, this,
    //    &FormInputStreamViews::onNewAcquisitionPose); QObject::connect(this,
    //    &FormInputStreamViews::newAcquisitionScan, this,
    //    &FormInputStreamViews::onNewAcquisitionScan);

    QObject::connect(this,
        &FormInputStreamViews::newAcquisition,
        this,
        &FormInputStreamViews::onNewAcquisition);

    //    QObject::connect(this,
    //        &FormInputStreamViews::newAcquisitionScan,
    //        this,
    //        &FormInputStreamViews::onNewAcquisitionScan);

    //    m_counterFpsThreads[1].start();
    //    m_counterFpsThreads[0].start();
    m_counterFpsThreads[1] = new CounterFpsThread(*ui->label_poseFrequency, "pose");
    m_counterFpsThreads[1]->start();
    m_counterFpsThreads[0] = new CounterFpsThread(*ui->label_scanFrequency, "scan");
    m_counterFpsThreads[0]->start();
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
    delete m_counterFpsThreads[1];
    m_counterFpsThreads[1] = nullptr;
    delete m_counterFpsThreads[0];
    m_counterFpsThreads[0] = nullptr;

    //    m_inputStreams.clear();
    //    m_threads.clear();

    delete ui;
    std::cout << "[FormInputStreamViews] ~FormInputStreamViews() end" << std::endl;
}

void FormInputStreamViews::onKillInputStream(const std::string& streamName)
{

    int iSensor = -1;
    bool found = false;
    while (!found && iSensor < 2) {
        ++iSensor;

        auto sensorName = m_sensorNames[iSensor];
        found = streamName.substr(0, sensorName.size()) == sensorName;
    }
    assert(found);

    onKillInputStreamPrivate(streamName, iSensor);
}

void FormInputStreamViews::onKillInputStreamPrivate(const std::string& streamName, int iSensor)
{
    std::cout << "[FormInputStreamViews] onKillInputStreamPrivate(" << streamName << ")" << std::endl;

    auto& streamsKilled = m_streamsKilled[iSensor];

    if (streamsKilled.find(streamName) != streamsKilled.end()) {
        streamsKilled.erase(streamName);
        return;
    }
    onDeleteInputStream(streamName, iSensor);

    //    assert(m_threads.find(streamName) != m_threads.end());
    //    auto& inputStreamThread = *m_threads.at(streamName);
    //    assert(inputStreamThread.isRunning());
    //    inputStreamThread.requestInterruption();
    //    inputStreamThread.wait();
}

void FormInputStreamViews::onNewAcquisition(const std::string& streamName, int iSensor)
{
    //    assert(m_activeThreadInputStreams[1] != nullptr);
    if (m_activeThreadInputStreams[iSensor] == nullptr)
        return;

    assert(m_activeStreamNames[iSensor] != "");
    assert(streamName == m_activeStreamNames[iSensor]);

    //    assert( m_recorder.isRecording() );
    if (m_recorder.isRecording()) {
        //    m_recorder.savePose(getPoseAcquisition());
        m_recorder.add(
            //        Snapshot( m_activeThreadInputStreams[iSensor]->mInputStream, m_activeThreadInputStreams[iSensor]->mAcq )
            //        );
            Snapshot(m_sensorNames[iSensor],
                m_activeThreadInputStreams[iSensor]->mInputStream.getFormat(),
                m_activeThreadInputStreams[iSensor]->mInputStream.getDims(),
                m_activeThreadInputStreams[iSensor]->mAcq));
        //    m_recorder.save(m_activeStreamNames[iSensor], m_activeThreadInputStreams[iSensor]->mAcq);
    }

    m_lastAcqs[iSensor] = m_activeThreadInputStreams[iSensor]->mAcq.clone();

    //    ++mCounterFramePose;
    ++m_counterFpsThreads[iSensor]->m_counterFrame;

    if (iSensor == 0) {
        emit newAcquisitionScan();
    } else if (iSensor == 1) {
        emit newAcquisitionPose();
    }
}

void FormInputStreamViews::onNewInputStream(const std::string& streamName, int iSensor)
{

    //    QComboBox & comboBox = ui->comboBox_pose;

    const auto sensorName = m_sensorNames[iSensor];

    assert(streamName.substr(0, sensorName.size()) == sensorName);

    //    auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
    //    if (sensorName == g_probeScanSensorName) {

    const auto sourceType = (sensorName.size() == streamName.size())
        ? ("physical")
        : (streamName.substr(sensorName.size() + 2,
            streamName.size() - 1 - sensorName.size() - 2));

    assert(sourceType == "" || sourceType == "physical" || sourceType == "record");

    //        QObject::connect(comboBox, &QComboBox::currentTextChanged, this,
    //        &FormInputStreamViews::onComboBox_scan_currentTextChanged);
    //        ui->comboBox_scan->addItem(sourceType.c_str());

    //        auto & stringList =
    //        m_streamModels[0].setStringList()

    auto stringList = m_streamModels[iSensor].stringList();
    for (const auto& string : stringList) {
        if (string == sourceType.c_str()) {
            return;
        }
    }
    stringList.append(sourceType.c_str());
    m_streamModels[iSensor].setStringList(stringList);

    //        if (m_streamModels[0].insertRow(m_streamModels[0].rowCount())) {
    //            auto index = m_streamModels[0].index(m_streamModels[0].rowCount() - 1, 0);
    //            m_streamModels[0].setData(index, sourceType.c_str());
    //        }
    //    }
}

void FormInputStreamViews::onDeleteInputStream(const std::string& streamName, int iSensor)
{
    std::cout << "[FormInputStreamViews] onDeleteInputStream(" << streamName << ")" << std::endl;

    auto& threads = m_threads[iSensor];

    //    auto sensorName = streamName.substr(0, g_probeScanSensorName.size());
    auto sensorName = m_sensorNames[iSensor];
    //    if (sensorName == g_probeScanSensorName) {

    QObject::disconnect(threads.at(streamName).get(),
        &InputStreamThread::newAcquisition,
        this,
        &FormInputStreamViews::newAcquisition);

    const auto sourceType = (sensorName.size() == streamName.size())
        ? ("physical")
        : (streamName.substr(sensorName.size() + 2,
            streamName.size() - 1 - sensorName.size() - 2));

    assert(sourceType == "" || sourceType == "physical" || sourceType == "record");
    //        QObject::connect(comboBox, &QComboBox::currentTextChanged, this,
    //        &FormInputStreamViews::onComboBox_scan_currentTextChanged);
    //        ui->comboBox_scan->addItem(sourceType.c_str());

    //        auto & stringList =
    //        m_streamModels[0].setStringList()
    auto stringList = m_streamModels[iSensor].stringList();
    //        stringList.append(sourceType.c_str());
    //        stringList.remove(sourceType.c_str());
    stringList.removeAll(sourceType.c_str());
    m_streamModels[iSensor].setStringList(stringList);

    // empty model do not imply the text view change
    if (stringList.empty())
        onComboBox_currentTextChanged("", iSensor);

    //        if (m_streamModels[0].insertRow(m_streamModels[0].rowCount())) {
    //            auto index = m_streamModels[0].index(m_streamModels[0].rowCount() - 1, 0);
    //            m_streamModels[0].setData(index, sourceType.c_str());
    //        }
    //    }

    assert(threads.find(streamName) != threads.end());
    threads.erase(streamName);

    auto& inputStreams = m_inputStreams[iSensor];

    assert(inputStreams.find(streamName) != inputStreams.end());
    inputStreams.erase(streamName);
}

const Stream::Acquisition& FormInputStreamViews::getScanAcquisition() const
{
    //    return m_thre
    //    if (m_threads.find(m_activeStreamNames[0]) == m_threads.end())
    //        return ;

    //    assert(m_threads.find(m_activeStreamNames[0]) != m_threads.end());
    //    return m_threads.at(m_activeStreamNames[0])->mAcq;

    return m_lastAcqs[0];
}

const Stream::Acquisition& FormInputStreamViews::getPoseAcquisition() const
{
    //    assert(m_threads.find(m_activeStreamNames[1]) != m_threads.end());
    //    return m_threads.at(m_activeStreamNames[1])->mAcq;

    return m_lastAcqs[1];
}

void FormInputStreamViews::onComboBox_currentTextChanged(const QString& sourceType, int iSensor)
{
    std::cout << "[FormInputStreamViews] on_comboBox_currentTextChanged('"
              << sourceType.toStdString() << "', " << iSensor << ")" << std::endl;

    assert(sourceType == "" || sourceType == "physical" || sourceType == "record");

    if (sourceType == "") {
        assert(m_activeStreamNames[iSensor] != "");
        QObject::disconnect(m_activeThreadInputStreams[iSensor],
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::newAcquisition);
        m_activeThreadInputStreams[iSensor] = nullptr;

        if (iSensor == 0)
            emit initScan();
        else
            emit initPose();
        m_activeStreamNames[iSensor] = "";
        return;
    }

    //            m_comp->initScan();
    const auto& streamName = (sourceType == "physical")
        ? (m_sensorNames[iSensor])
        : (m_sensorNames[iSensor] + " (" + sourceType.toStdString() + ")");

    if (m_activeStreamNames[iSensor] == streamName)
        return;

    //    std::cout << "[onComboBox_currentTextChanged] '" << streamName << "'" << ", iSensor = " << iSensor << std::endl;
    //    assert(m_sensorViews != nullptr);

    if (m_activeStreamNames[iSensor] != "" && streamName != m_activeStreamNames[iSensor]) {
        assert(m_activeThreadInputStreams[iSensor] != nullptr);
        QObject::disconnect(m_activeThreadInputStreams[iSensor],
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::newAcquisition);
    }

    auto& threads = m_threads[iSensor];
    assert(threads.find(streamName) != threads.end());
    //    } else {
    //            const auto& sensorView = m_sensorViews->getSensorView(streamName);
    m_activeThreadInputStreams[iSensor] = threads.at(streamName).get();

    QObject::connect(m_activeThreadInputStreams[iSensor],
        &InputStreamThread::newAcquisition,
        this,
        &FormInputStreamViews::newAcquisition);
    //    }
    m_activeStreamNames[iSensor] = streamName;
}

void FormInputStreamViews::on_comboBox_scan_currentTextChanged(const QString& sourceType)
{
    std::cout << "[FormInputStreamViews] on_comboBox_scan_currentTextChanged('"
              << sourceType.toStdString() << "')" << std::endl;

    onComboBox_currentTextChanged(sourceType, 0);
}

void FormInputStreamViews::on_comboBox_pose_currentTextChanged(const QString& sourceType)
{
    std::cout << "[FormInputStreamViews] on_comboBox_pose_currentTextChanged('"
              << sourceType.toStdString() << "')" << std::endl;

    onComboBox_currentTextChanged(sourceType, 1);
}

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

void FormInputStreamViews::on_toolButton_snapshot_clicked()
{
    assert(!m_recorder.isRecording());
    Frame frame;

    if (m_activeStreamNames[0] != "") {
        Snapshot scanSnapshot(m_activeThreadInputStreams[0]->mInputStream,
            m_activeThreadInputStreams[0]->mAcq);
        frame.push_back(scanSnapshot);
    }
    if (m_activeStreamNames[1] != "") {
        Snapshot poseSnapshot(m_activeThreadInputStreams[1]->mInputStream,
            m_activeThreadInputStreams[1]->mAcq);
        frame.push_back(poseSnapshot);
    }
    if (frame.empty()) {
        return;
    }
    m_recorder.save(frame);
}
