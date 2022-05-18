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

class InputStreamThread : public QThread {
    Q_OBJECT
public:
    // constructor

    //    template <class IOStreamT>
    //    InputStreamThread(IOStreamT&& iostream, QObject* parent = nullptr);
    InputStreamThread(InputStream& iostream, QObject* parent = nullptr);

    ~InputStreamThread();

signals:
    void newAcquisition(std::string sensorName);
    void streamingStopped(std::string sensorName);

public:
    // overriding the QThread's run() method
    void run();

    Stream::Acquisition mAcq;
    InputStream& mInputStream;
    std::string mSensorName;

private:
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
    void newAcquisitionPose();
    void newAcquisitionScan();

public slots:
//    void onComboBox_scan_currentTextChanged(const QString& sourceType);
//    void onComboBox_pose_currentTextChanged(const QString& sourceType);

    //    void onNewAcquisition(const std::string & sensorName);
//    void onNewScanAcquisition(const std::string& sensorName);
//    void onNewPoseAcquisition(const std::string& sensorName);
    void onKillInputStream(const std::string & streamName);

private slots:

    void onNewInputStream(std::string streamName);
    void onDeleteInputStream(const std::string& streamName);
    //    void on_stopStreaming(std::string streamName);

    void on_comboBox_scan_currentTextChanged(const QString &sourceType);
    void on_comboBox_pose_currentTextChanged(const QString &sourceType);

    void on_comboBox_scan_currentIndexChanged(int index);
    void on_comboBox_pose_currentIndexChanged(int index);

    void on_toolButton_record_clicked();

    void on_toolButton_snapshot_clicked();

private:
    Ui::FormInputStreamViews* ui;

    const InputStreamThread* m_threadInputStreamScan = nullptr;
    std::string m_activeStreamScan = "";
    QStringListModel m_scanModel;

    const InputStreamThread* m_threadInputStreamPose = nullptr;
    std::string m_activeStreamPose = "";
    QStringListModel m_poseModel;

    std::map<std::string, std::unique_ptr<InputStream>> m_inputStreams;

    //    std::map<std::string, QHBoxLayout*> m_hBoxLayouts;
    //    std::map<std::string, std::unique_ptr<InputStreamThread>> m_threads;
    std::map<std::string, std::unique_ptr<InputStreamThread>> m_threads;

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
    assert(m_inputStreams.find(streamName) == m_inputStreams.end());
    m_inputStreams[streamName] = std::make_unique<InputStream>(std::move(iostream));

    auto& inputStream = *m_inputStreams.at(streamName);
    assert(inputStream.getSensorName() == streamName);

    //    InputStream inputStream(std::move(iostream));
    //    const std::string & streamName = inputStream.getSensorName();
    assert(m_threads.find(streamName) == m_threads.end());
    m_threads[streamName] = std::make_unique<InputStreamThread>(inputStream);

    //    m_threads[streamName] = std::make_unique<InputStreamThread>(std::move(iostream));
    //    m_threads[streamName] = std::make_unique<InputStreamThread>(std::forward<IOStreamT>(iostream));

    //    m_threads[streamName] = std::make_unique<InputStreamThread>(std::forward<IOStream&&>(iostream));

    //    m_threads[streamName] = std::make_unique<InputStreamThread>(std::forward<Args>(args)...);

    auto& inputStreamThread = *m_threads.at(streamName);
    //    assert(inputStreamThread->mInputStream.getSensorName() == streamName);
    //            QObject::connect(inputStreamThread, &InputStreamThread::newAcquisition, this, &FormInputStreamViews::onNewAcquisition);
    QObject::connect(
        &inputStreamThread, &InputStreamThread::streamingStopped, this, &FormInputStreamViews::onDeleteInputStream);

    inputStreamThread.start();

    emit onNewInputStream(streamName);
    //    //    m_inputStreams["ouou"] = std::make_unique<InputStream>(std::move(iostream));

    //    m_inputStreams[streamName] = std::make_unique<InputStream>(std::move(iostream));

    //    auto& inputStream = *m_inputStreams.at(streamName);
    //    assert(inputStream.getSensorName() == streamName);

    //    //        m_threads[streamName] = std::make_unique<InputStreamThread>(InputStreamThread(*m_inputStreams[streamName]));
    //    auto* inputStreamThread = new InputStreamThread(inputStream);

    //    assert(m_threads.find(streamName) == m_threads.end());
    //    m_threads[streamName] = inputStreamThread;

    //        m_threads[streamName] = InputStreamThread(RamIO(cyclicBuff));
    //        InputStream()

    //        on_startStreaming(streamName);
}

#endif // FORMINPUTSTREAMVIEWS_H
