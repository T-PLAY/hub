#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MainWindowStreamView.h>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QThread>
#include <map>
#include <server.h>

// Q_DECLARE_METATYPE(Streamer);
//Q_DECLARE_METATYPE(std::string)

namespace Ui {
class MainWindow;
}

class Thread_Client : public QThread {
    Q_OBJECT
public:
    // constructor
    explicit Thread_Client(QObject* parent = 0);

signals:
    void addViewStreamSignal(std::string streamerSensorName);
    void delViewStreamSignal(std::string streamerSensorName);

public:
    // overriding the QThread's run() method
    void run();

private:
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:

public slots:
    void addViewStream(std::string streamerSensorName);
    void delViewStream(std::string streamerSensorName);

private:
    Ui::MainWindow* ui;
    Thread_Client mThreadClient;
    std::list<MainWindowStreamView*> mStreamViews;
};

#endif // MAINWINDOW_H
