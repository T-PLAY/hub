#ifndef MAINWINDOWSTREAMVIEW_H
#define MAINWINDOWSTREAMVIEW_H

#include <QMainWindow>
#include <QThread>
#include <chrono>
#include <stream.h>

class Thread_InputStream : public QThread {
    Q_OBJECT
public:
    // constructor
    explicit Thread_InputStream(QObject* parent = 0, std::string sensorName = "");
    ~Thread_InputStream();

signals:
    //    void addViewStreamSignal(Streamer streamer);
    //    void addViewStreamSignal(int iStreamer);
    //    void addViewStreamSignal();
    void newImage();

public:
    // overriding the QThread's run() method
    void run();

//    Stream::InitPacket mInitPacket;
    unsigned char* mData[2];
    int m_iReadBuffer = 0;
    int m_iWriteBuffer = 0;

    InputStream mInputStream;
private:
//    ClientSocket mSock;
//    int mAcquisitionSize;

    //    Ui::MainWindow * mUi;
};

/////////////////////////////////////////////////////////////////////////////


namespace Ui {
class MainWindowStreamView;
}

class MainWindowStreamView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowStreamView(QWidget *parent = nullptr, std::string sensorName = "");
    ~MainWindowStreamView();

    std::string getStreamerSensorName() const;

public slots:
    void newImage();

private:
    Ui::MainWindowStreamView *ui;

//    int m_iStreamer;
    Thread_InputStream mThread;

    int mCounterFps = 0;
    double mFps = 1.0;
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartFps;
};

#endif // MAINWINDOWSTREAMVIEW_H
