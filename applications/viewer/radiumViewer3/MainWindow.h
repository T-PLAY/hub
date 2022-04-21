#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <Core/Tasks/TaskQueue.hpp>
#include <Gui/Viewer/Viewer.hpp>
#include <stream.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Thread_Client : public QThread {
    Q_OBJECT
public:
    // constructor
    Thread_Client(QObject* parent, Ra::Gui::Viewer& m_viewer, Ra::Engine::RadiumEngine& engine);

signals:
    //    void addSensorSignal(std::string streamerSensorName, std::string format, std::string dims, std::string size, std::string metaData);
    //    void delSensorSignal(std::string streamerSensorName);

public:
    // overriding the QThread's run() method
    void run();

    Ra::Engine::Rendering::RenderObject* m_scan = nullptr;
    Ra::Engine::Rendering::RenderObject* m_probe = nullptr;
    Ra::Engine::Rendering::RenderObject* m_probe_axis[3] = { nullptr, nullptr, nullptr };

private:
    size_t m_acquisition = 0;
    Ra::Gui::Viewer* m_viewer;
    Ra::Engine::RadiumEngine* m_engine;

    InputStream* m_poseStream = nullptr;
    InputStream* m_scanStream = nullptr;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:

    /// This function is the basic "game loop" iteration of the engine.
    /// It starts the rendering then advance all systems by one frame.
    void onGLInitialized();
    void frame();

public:
    void initScene();

private:
    Ui::MainWindow* ui;

    // Our instance of the engine
    Ra::Engine::RadiumEngine* m_engine = nullptr;

    // Task queue
    Ra::Core::TaskQueue* m_task_queue = nullptr;

    // Pointer to Qt/OpenGL Viewer widget
    Ra::Gui::Viewer* m_viewer = nullptr;

    // Pointer to Viewer widget
    QWidget* m_viewerWidget = nullptr;

    // Timer to wake us up at every frame start.
    QTimer* m_frame_timer;

    // Our framerate
    uint m_target_fps = 60;

    Thread_Client* m_threadClient = nullptr;


    //    Ra::Engine::Rendering::RenderObject* m_scan = nullptr;
    //    Ra::Engine::Rendering::RenderObject* m_probe = nullptr;
    //    Ra::Engine::Rendering::RenderObject* m_probe_axis[3] = { nullptr, nullptr, nullptr };
};
#endif // MAINWINDOW_H
