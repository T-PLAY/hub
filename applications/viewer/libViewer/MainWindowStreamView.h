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
    void newImage();

public:
    // overriding the QThread's run() method
    void run();

//    unsigned char* mData[2];
    unsigned char* mData = nullptr;
//    int m_iReadBuffer = 0;
//    int m_iWriteBuffer = 0;

    InputStream mInputStream;

private:
};

/////////////////////////////////////////////////////////////////////////////

namespace Ui {
class MainWindowStreamView;
}

class MainWindowStreamView : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindowStreamView(QWidget* parent = nullptr, std::string sensorName = "");
    ~MainWindowStreamView();

    std::string getStreamerSensorName() const;

signals:
    void onCloseStreamViewSignal(std::string streamerSensorName);

public slots:
    void newImage();

private:
    Ui::MainWindowStreamView* ui;

    Thread_InputStream mThread;

    int mCounterFps = 0;
    double mFps = 10.0;
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartFps;
    std::string mSensorName;
};

#endif // MAINWINDOWSTREAMVIEW_H
