#ifndef FORMINPUTSTREAMVIEWS_H
#define FORMINPUTSTREAMVIEWS_H

#include <QHBoxLayout>
#include <QWidget>
#include <RamIO.h>
#include <stream.h>

#include <QThread>

class InputStreamThread : public QThread {
    Q_OBJECT
public:
    // constructor

    template <class IOStream>
    InputStreamThread(IOStream&& iostream, QObject* parent = nullptr);

    ~InputStreamThread();

signals:
    void newAcquisition(std::string sensorName);

    void streamingStopped(std::string sensorName);

public:
    // overriding the QThread's run() method
    void run();

    Stream::Acquisition mAcq;
    InputStream mInputStream;
    std::string mSensorName;

private:
};

template <class IOStream>
InputStreamThread::InputStreamThread(IOStream&& iostream, QObject* parent)
    : QThread(parent)
    , mInputStream(std::move(iostream))
    , mSensorName(mInputStream.getSensorName())
{
}

namespace Ui {
class FormInputStreamViews;
}

class FormInputStreamViews : public QWidget {
    Q_OBJECT

public:
    explicit FormInputStreamViews(QWidget* parent = nullptr);
    ~FormInputStreamViews();

    void connect(const std::map<std::string, std::unique_ptr<CyclicBuff>>& ramOutputStreams);
    template <class IOStream>
    void onNewInputStream(const std::string streamName, IOStream&& iostream);

signals:
    void initPose();
    void initScan();
    void updatePose();
    void updateScan();

public slots:
    void onComboBox_scan_currentTextChanged(const QString& sourceType);
    void onComboBox_pose_currentTextChanged(const QString& sourceType);

    //    void onNewAcquisition(const std::string & sensorName);
    void onNewScanAcquisition(const std::string& sensorName);
    void onNewPoseAcquisition(const std::string& sensorName);

private slots:

    //    void on_startStreaming(std::string streamName);
    //    void on_stopStreaming(std::string streamName);
    void onDeleteInputStream(const std::string& streamName);

private:
    Ui::FormInputStreamViews* ui;

    const InputStreamThread* m_threadInputStreamScan = nullptr;
    std::string m_activeStreamScan = "";
    const InputStreamThread* m_threadInputStreamPose = nullptr;
    std::string m_activeStreamPose = "";

//    std::map<std::string, std::unique_ptr<InputStream>> m_inputStreams;

    //    std::map<std::string, QHBoxLayout*> m_hBoxLayouts;
    //    std::map<std::string, std::unique_ptr<InputStreamThread>> m_threads;
    std::map<std::string, std::unique_ptr<InputStreamThread>> m_threads;

public:
    const Stream::Acquisition& getScanAcquisition() const;
    const Stream::Acquisition& getPoseAcquisition() const;
};

template <class IOStream>
void FormInputStreamViews::onNewInputStream(const std::string streamName, IOStream&& iostream)
{
    //    InputStream inputStream(std::move(iostream));
    //    const std::string & streamName = inputStream.getSensorName();
    assert(m_threads.find(streamName) == m_threads.end());
    m_threads[streamName] = std::make_unique<InputStreamThread>(std::move(iostream));

    const auto & inputStreamThread = m_threads.at(streamName);
    assert(inputStreamThread->mInputStream.getSensorName() == streamName);

    inputStreamThread->start();

//    //    m_inputStreams["ouou"] = std::make_unique<InputStream>(std::move(iostream));
//    assert(m_inputStreams.find(streamName) == m_inputStreams.end());
//    //        m_inputStreams[streamName] = std::make_unique<InputStream>(RamIO(cyclicBuff));
//    m_inputStreams[streamName] = std::make_unique<InputStream>(std::move(iostream));


//    auto& inputStream = *m_inputStreams.at(streamName);
//    assert(inputStream.getSensorName() == streamName);

//    //        m_threads[streamName] = std::make_unique<InputStreamThread>(InputStreamThread(*m_inputStreams[streamName]));
//    auto* inputStreamThread = new InputStreamThread(inputStream);
    //        QObject::connect(inputStreamThread, &InputStreamThread::newAcquisition, this, &FormInputStreamViews::onNewAcquisition);

//    QObject::connect(
//        inputStreamThread, &InputStreamThread::streamingStopped, this, &FormInputStreamViews::onDeleteInputStream);

//    assert(m_threads.find(streamName) == m_threads.end());
//    m_threads[streamName] = inputStreamThread;


    //        m_threads[streamName] = InputStreamThread(RamIO(cyclicBuff));
    //        InputStream()

    //        on_startStreaming(streamName);
}

#endif // FORMINPUTSTREAMVIEWS_H
