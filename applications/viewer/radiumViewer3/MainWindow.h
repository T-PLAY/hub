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

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow( QWidget* parent = nullptr );
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

    //    Thread_Client* m_threadClient = nullptr;

    InputStream* m_poseStream                            = nullptr;
    InputStream* m_scanStream                            = nullptr;
    Ra::Engine::Rendering::RenderObject* m_scan          = nullptr;
    Ra::Engine::Rendering::RenderObject* m_probe         = nullptr;
    Ra::Engine::Rendering::RenderObject* m_probe_axis[3] = { nullptr, nullptr, nullptr };
};
#endif // MAINWINDOW_H
