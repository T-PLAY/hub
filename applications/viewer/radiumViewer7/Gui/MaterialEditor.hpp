#ifndef RADIUMENGINE_MATERIALEDITOR_HPP
#define RADIUMENGINE_MATERIALEDITOR_HPP

#include <QWidget>

#include <memory>

#include <Core/Utils/Index.hpp>

#include <ui_MaterialEditor.h>

class QCloseEvent;
class QShowEvent;

namespace Ra {
namespace Engine {
namespace Rendering{
class RadiumEngine;
class RenderObjectManager;
class RenderObject;
}
namespace Data {
class Material;
class BlinnPhongMaterial;
} // namespace Data
} // namespace Engine
} // namespace Ra

namespace Ra {
namespace Gui {
class MaterialEditor : public QWidget, private Ui::MaterialEditor
{
    Q_OBJECT

  public:
    explicit MaterialEditor( QWidget* parent = nullptr );

    void changeRenderObject( Ra::Core::Utils::Index roIdx );

  signals:
    void materialChanged();

  private slots:

    void updateBlinnPhongViz();

    void onKdColorChanged( int );
    void onKsColorChanged( int );

    void onExpChanged( double );

    void newKdColor( const QColor& color );
    void newKsColor( const QColor& color );

    void on_m_closeButton_clicked();
    void on_kUsePerVertex_clicked( bool checked );

  protected:
    virtual void showEvent( QShowEvent* e ) override;
    virtual void closeEvent( QCloseEvent* e ) override;
    void updateEngine();

  private:
    bool m_visible;

    Core::Utils::Index m_roIdx;
    std::shared_ptr<Engine::Rendering::RenderObject> m_renderObject;

    /// TODO generalize material editor to others materials
    bool m_usable;
    Ra::Engine::Data::BlinnPhongMaterial* m_blinnphongmaterial;

  private:
    enum {
        OUTPUT_FINAL    = 0,
        OUTPUT_DIFFUSE  = 1,
        OUTPUT_SPECULAR = 2,
        OUTPUT_NORMAL   = 3,
    };
};
} // namespace Gui
} // namespace Ra

#endif // RADIUMENGINE_MATERIALEDITOR_HPP
