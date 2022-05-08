#ifndef MAINWINDOWSTREAMVIEW_H
#define MAINWINDOWSTREAMVIEW_H

#include <QMainWindow>
//#include <QThread>
//#include <chrono>
#include <stream.h>

/////////////////////////////////////////////////////////////////////////////

namespace Ui {
class MainWindowStreamView;
}

class MainWindowStreamView : public QMainWindow {
    Q_OBJECT

public:
    MainWindowStreamView(const InputStream & inputStream, QWidget* parent = nullptr);
    ~MainWindowStreamView();

//    std::string getStreamerSensorName() const;

    void setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format);

signals:
    void onCloseStreamViewSignal();

public slots:

private:
    Ui::MainWindowStreamView* ui;

//    Thread_InputStream mThread;
    const InputStream & m_inputStream;

//    std::string mSensorName;
};

#endif // MAINWINDOWSTREAMVIEW_H
