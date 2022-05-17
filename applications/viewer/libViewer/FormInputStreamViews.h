#ifndef FORMINPUTSTREAMVIEWS_H
#define FORMINPUTSTREAMVIEWS_H

#include <QWidget>
#include <stream.h>
#include <QHBoxLayout>
#include <RamIO.h>

#include <QThread>

class InputStreamThread : public QThread
{
    Q_OBJECT
  public:
    // constructor
    explicit InputStreamThread( InputStream & inputStream, QObject* parent = nullptr );
    ~InputStreamThread();

  signals:
    void newAcquisition(std::string sensorName);
//    void streamingStopped(std::string sensorName);

  public:
    // overriding the QThread's run() method
    void run();

    Stream::Acquisition mAcq;
    InputStream & mInputStream;
    std::string mSensorName;

  private:
};


namespace Ui {
class FormInputStreamViews;
}

class FormInputStreamViews : public QWidget
{
    Q_OBJECT

public:
    explicit FormInputStreamViews(QWidget *parent = nullptr);
    ~FormInputStreamViews();


    void connect(const std::map<std::string, CyclicBuff> & ramOutputStreams);

signals:
    void initPose();
    void initScan();
    void updatePose();
    void updateScan();

public slots:
    void onComboBox_scan_currentTextChanged(const QString & sourceType);
    void onComboBox_pose_currentTextChanged(const QString & sourceType);

//    void onNewAcquisition(const std::string & sensorName);
    void onNewScanAcquisition(const std::string & sensorName);
    void onNewPoseAcquisition(const std::string & sensorName);

private:
    Ui::FormInputStreamViews *ui;

    const InputStreamThread * m_threadInputStreamScan = nullptr;
    std::string m_activeStreamScan = "";
    const InputStreamThread * m_threadInputStreamPose = nullptr;
    std::string m_activeStreamPose = "";


    std::map<std::string, std::unique_ptr<InputStream>> m_inputStreams;
    std::map<std::string, QHBoxLayout*> m_hBoxLayouts;
//    std::map<std::string, std::unique_ptr<InputStreamThread>> m_threads;
    std::map<std::string, InputStreamThread*> m_threads;

public:
    const Stream::Acquisition & getScanAcquisition() const;
    const Stream::Acquisition & getPoseAcquisition() const;

};

#endif // FORMINPUTSTREAMVIEWS_H
