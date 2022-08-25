#ifndef MAINWINDOWSTREAMVIEW_H
#define MAINWINDOWSTREAMVIEW_H

#include <QMainWindow>
//#include <QThread>
//#include <chrono>

#include <InputSensor.hpp>

/////////////////////////////////////////////////////////////////////////////

namespace Ui {
class MainWindowStreamView;
}

class MainWindowStreamView : public QMainWindow {
    Q_OBJECT

public:
    MainWindowStreamView(const hub::InputSensor & inputStream, QWidget* parent = nullptr);
    ~MainWindowStreamView();

//    std::string getStreamerSensorName() const;

    void setData(unsigned char* img_ptr, std::vector<int> dims, hub::SensorSpec::Format format);

signals:
    void onCloseStreamViewSignal();

public slots:

private:
    Ui::MainWindowStreamView* ui;

//    Thread_InputStream mThread;
    const hub::InputSensor & m_inputStream;

//    std::string mSensorName;
};

#endif // MAINWINDOWSTREAMVIEW_H
