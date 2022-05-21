#ifndef FORMINPUTSTREAMVIEWS_H
#define FORMINPUTSTREAMVIEWS_H

#include <QHBoxLayout>
#include <QWidget>
#include <RamIO.h>
#include <stream.h>

#include <QObject>
#include <QThread>
//#include <constants.h>
//#include <QComboBox>
#include <QStringListModel>
#include <Recorder.h>
//#include <list>
#include <QLabel>
#include <constants.h>

class FormInputStreamViews;

class InputStreamThread : public QThread {
    Q_OBJECT
public:
    // constructor

    //    template <class IOStreamT>
    //    InputStreamThread(IOStreamT&& iostream, QObject* parent = nullptr);
    InputStreamThread(InputStream& iostream, FormInputStreamViews& formInputStreamViews, int iSensor, QObject* parent = nullptr);

    ~InputStreamThread();

signals:
    void newAcquisition(const std::string& sensorName, int iSensor);
    void streamingStopped(const std::string& sensorName, int iSensor);

public:
    // overriding the QThread's run() method
    void run();

    Stream::Acquisition mAcq;
    int mIStream;
    InputStream& mInputStream;
    std::string mSensorName;
    FormInputStreamViews& mFormInputStreamViews;

private:
};

class CounterFpsThread : public QThread {
    Q_OBJECT
public:
    CounterFpsThread(QLabel& label, const std::string& sensorName);
    ~CounterFpsThread();
    // constructor
signals:

public:
    // overriding the QThread's run() method
    void run();

    size_t m_counterFrame = 0;

private:
    std::string m_sensorName;
    //    double m_fps = 10.0;
    QLabel& m_labelFps;
};

// template <class IOStreamT>
// InputStreamThread::InputStreamThread(IOStreamT&& iostream, QObject* parent)
//     : QThread(parent)
//     , mInputStream(std::make_unique<InputStream>(std::move(iostream)))
////    , mInputStream(std::forward<IOStream>(iostream))
////    , mSensorName(mInputStream.getSensorName())
//{
//}

namespace Ui {
class FormInputStreamViews;
}

class FormInputStreamViews : public QWidget {
    Q_OBJECT

public:
    explicit FormInputStreamViews(QWidget* parent = nullptr);
    ~FormInputStreamViews();

    //    void connect(const std::map<std::string, std::unique_ptr<CyclicBuff>>& ramOutputStreams);
    template <class IOStreamT>
    void addInputStream(const std::string streamName, IOStreamT&& iostream);
    //    template <typename ...Args>
    //    void addInputStream(const std::string streamName, Args&&... iostream);

signals:
    void initPose();
    void initScan();
    void newAcquisition(const std::string& streamName, int iSensor);
    void newAcquisitionScan();
    void newAcquisitionPose();

public slots:
    //    void onComboBox_scan_currentTextChanged(const QString& sourceType);
    //    void onComboBox_pose_currentTextChanged(const QString& sourceType);

    //    void onNewAcquisition(const std::string & sensorName);
    //    void onNewScanAcquisition(const std::string& sensorName);
    //    void onNewPoseAcquisition(const std::string& sensorName);
    void onKillInputStream(const std::string& streamName);

private slots:
    void onKillInputStreamPrivate(const std::string& streamName, int iSensor);

    void onNewAcquisition(const std::string& streamName, int iSensor);
    //        void onNewAcquisitionPose(const std::string& streamName);
    //        void onNewAcquisitionScan(const std::string& streamName);

    void onNewInputStream(const std::string& streamName, int iSensor);
    void onDeleteInputStream(const std::string& streamName, int iSensor);
    //    void on_stopStreaming(std::string streamName);

    void onComboBox_currentTextChanged(const QString& sourceType, int iSensor);
    void on_comboBox_scan_currentTextChanged(const QString& sourceType);
    void on_comboBox_pose_currentTextChanged(const QString& sourceType);

    //    void on_comboBox_scan_currentIndexChanged(int index);
    //    void on_comboBox_pose_currentIndexChanged(int index);

    void on_toolButton_record_clicked();
    void on_toolButton_snapshot_clicked();

private:
private:
    Ui::FormInputStreamViews* ui;

    const std::string m_sensorNames[2] = { g_probeScanSensorName, g_probePoseSensorName };
    const InputStreamThread* m_activeThreadInputStreams[2] = { nullptr, nullptr };
    std::string m_activeStreamNames[2] = { "", "" };
    QStringListModel m_streamModels[2];
    Stream::Acquisition m_lastAcqs[2];
    CounterFpsThread* m_counterFpsThreads[2] = { nullptr, nullptr };
    std::map<std::string, std::unique_ptr<InputStream>> m_inputStreams[2];
    std::map<std::string, std::unique_ptr<InputStreamThread>> m_threads[2];
    std::set<std::string> m_streamsKilled[2];

    Recorder m_recorder;

public:
    const Stream::Acquisition& getScanAcquisition() const;
    const Stream::Acquisition& getPoseAcquisition() const;
};

template <class IOStreamT>
void FormInputStreamViews::addInputStream(const std::string streamName, IOStreamT&& iostream)
// template <typename ...Args>
// void FormInputStreamViews::addInputStream(const std::string streamName, Args&&... args)
{
    std::cout << "[FormInputStreamViews] addInputStream('" << streamName << "')" << std::endl;

    //    while (m_inputStreams.find(streamName) != m_inputStreams.end()) {
    //        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //    }
    //    if (m_inputStreams.find(streamName) != m_inputStreams.end()) {
    //        onDeleteInputStream(streamName);
    //    }
    //       if (streamName.substr(0, m_sensorNames[0].size());
    int iSensor = -1;
    bool found = false;
    while (!found && iSensor < 2) {
        ++iSensor;

        const auto & sensorName = m_sensorNames[iSensor];
        found = streamName.substr(0, sensorName.size()) == sensorName;
    }
    assert(found);
    //    if (sensorName == g_probePoseSensorName) {

    auto & inputStreams = m_inputStreams[iSensor];

    auto & streamsKilled = m_streamsKilled[iSensor];

    if (inputStreams.find(streamName) != inputStreams.end()) {
        //        auto * inputStream  = inputStreams.at(streamName).get();
        //        delete inputStream;
        inputStreams.erase(streamName);
        //        assert(m_streamsKilled.find(streamName) == m_streamsKilled.end());
        streamsKilled.insert(streamName);
    }
    assert(inputStreams.find(streamName) == inputStreams.end());
    inputStreams[streamName] = std::make_unique<InputStream>(std::move(iostream));

    auto& inputStream = *inputStreams.at(streamName);
    assert(inputStream.getSensorName() == streamName);

    //    InputStream inputStream(std::move(iostream));
    //    const std::string & streamName = inputStream.getSensorName();

    auto & threads = m_threads[iSensor];

    if (threads.find(streamName) != threads.end()) {
        auto* thread = threads.at(streamName).get();
        QObject::disconnect(thread,
            &InputStreamThread::newAcquisition,
            this,
            &FormInputStreamViews::newAcquisition);
        //        QObject::disconnect( thread,
        //                             &InputStreamThread::streamingStopped,
        //                             this,
        //                             &FormInputStreamViews::onDeleteInputStream );
        //        delete thread;
        threads.erase(streamName);
    }
    assert(threads.find(streamName) == threads.end());
    threads[streamName] = std::make_unique<InputStreamThread>(inputStream, *this, iSensor);

    //    threads[streamName] = std::make_unique<InputStreamThread>(std::move(iostream));
    //    threads[streamName] = std::make_unique<InputStreamThread>(std::forward<IOStreamT>(iostream));

    //    threads[streamName] = std::make_unique<InputStreamThread>(std::forward<IOStream&&>(iostream));

    //    threads[streamName] = std::make_unique<InputStreamThread>(std::forward<Args>(args)...);

    auto* inputStreamThread = threads.at(streamName).get();
    //    assert(inputStreamThread->mInputStream.getSensorName() == streamName);
    //            QObject::connect(inputStreamThread, &InputStreamThread::newAcquisition, this, &FormInputStreamViews::onNewAcquisition);
    QObject::connect(
        inputStreamThread, &InputStreamThread::streamingStopped, this, &FormInputStreamViews::onKillInputStreamPrivate);

    inputStreamThread->start();

    //    emit onNewInputStream(streamName);
    onNewInputStream(streamName, iSensor);

    //    //    inputStreams["ouou"] = std::make_unique<InputStream>(std::move(iostream));

    //    inputStreams[streamName] = std::make_unique<InputStream>(std::move(iostream));

    //    auto& inputStream = *inputStreams.at(streamName);
    //    assert(inputStream.getSensorName() == streamName);

    //    //        threads[streamName] = std::make_unique<InputStreamThread>(InputStreamThread(*inputStreams[streamName]));
    //    auto* inputStreamThread = new InputStreamThread(inputStream);

    //    assert(threads.find(streamName) == threads.end());
    //    threads[streamName] = inputStreamThread;

    //        threads[streamName] = InputStreamThread(RamIO(cyclicBuff));
    //        InputStream()

    //        on_startStreaming(streamName);
}

#endif // FORMINPUTSTREAMVIEWS_H
