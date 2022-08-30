

#include <SceneComponent.hpp>

#include <Core/Asset/FileData.hpp>
#include <Core/Containers/MakeShared.hpp>
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Core/Geometry/TopologicalMesh.hpp>
#include <Core/Resources/Resources.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>
#include <Core/Utils/Timer.hpp>

#include <Engine/Data/BlinnPhongMaterial.hpp>
#include <Engine/Data/LambertianMaterial.hpp>
#include <Engine/Data/Mesh.hpp>
#include <Engine/Data/PlainMaterial.hpp>
#include <Engine/FrameInfo.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>

#ifdef IO_USE_ASSIMP
#    include <IO/AssimpLoader/AssimpFileLoader.hpp>
#endif

#include <random>

const bool ENABLE_GRID      = true;

using namespace Ra;
using namespace Ra::Core;
using namespace Ra::Core::Utils;
using namespace Ra::Core::Geometry;
using namespace Ra::Engine;
using namespace Ra::Engine::Rendering;
using namespace Ra::Engine::Data;
using namespace Ra::Engine::Scene;

/**
 * This file contains a minimal radium/qt application which shows the geometrical primitives
 * supported by Radium
 */

SceneComponent::SceneComponent( Ra::Engine::Scene::Entity* entity ) :
    Ra::Engine::Scene::Component( "Scene component", entity ) {}


/// This function is called when the component is properly
/// setup, i.e. it has an entity.
void SceneComponent::initialize() {
//    auto blinnPhongMaterial              = make_shared<BlinnPhongMaterial>( "Shaded Material" );
//    blinnPhongMaterial->m_perVertexColor = true;
//    blinnPhongMaterial->m_ks             = Color::White();
//    blinnPhongMaterial->m_ns             = 100_ra;

    auto plainMaterial              = make_shared<PlainMaterial>( "Plain Material" );
    plainMaterial->m_perVertexColor = true;

//    auto lambertianMaterial              = make_shared<LambertianMaterial>( "Lambertian Material" );
//    lambertianMaterial->m_perVertexColor = true;

    //// setup ////
    Scalar cellSize   = 100_ra;
    int nCellX        = 10;

    //// GRID ////
    if ( ENABLE_GRID ) {

        auto gridPrimitive = DrawPrimitives::Grid( Vector3::Zero(),
                                                   Vector3::UnitX(),
                                                   Vector3::UnitZ(),
                                                   Color::Grey( 0.6f ),
                                                   cellSize,
                                                   nCellX );

        assert(m_roGrid == nullptr);
        m_roGrid = RenderObject::createRenderObject(
            "grid", this, RenderObjectType::Geometry, gridPrimitive, {} );
        m_roGrid->setMaterial( Ra::Core::make_shared<PlainMaterial>( "Grid material" ) );
        m_roGrid->setPickable( false );
        addRenderObject( m_roGrid );
    }

    // origin gizmo
    {
        std::vector<std::shared_ptr<Engine::Data::Mesh>> m_meshAxis;
        constexpr Scalar arrowScale = 50_ra;
        constexpr Scalar axisWidth = 0.05_ra;
        constexpr Scalar arrowFrac = 0.0_ra;

        std::vector<Color> gizmoColors = {
            Color::Red(), Color::Green(), Color::Blue()
        };

        for (uint i = 0; i < 3; ++i) {
            Core::Vector3 cylinderEnd = Core::Vector3::Zero();
            cylinderEnd[i % 3] = (1_ra - arrowFrac);
            Core::Vector3 arrowEnd = Core::Vector3::Zero();
            arrowEnd[i % 3] = 1_ra;
            Core::Geometry::TriangleMesh cylinder = Core::Geometry::makeCylinder(Core::Vector3::Zero(),
                arrowScale * cylinderEnd,
                arrowScale * axisWidth / 2_ra,
                32,
                gizmoColors[i]);

            std::shared_ptr<Engine::Data::Mesh> mesh(
                new Engine::Data::Mesh("Translate Gizmo Arrow"));
            mesh->loadGeometry(std::move(cylinder));

            m_meshAxis.push_back(std::move(mesh));
        }

        // origin axis
        assert(m_roAxes[1] == nullptr);
        for (uint i = 0; i < 3; ++i) {
            m_roAxes[i] = RenderObject::createRenderObject("originAxis" + std::to_string(i),
                this,
                RenderObjectType::Geometry,
                m_meshAxis[i]);

            m_roAxes[i]->setMaterial(plainMaterial);
//            Ra::Core::Transform TLocal = Transform::Identity();
//            TLocal.scale(100.0);
//            m_roAxes[i]->setLocalTransform(TLocal);
            addRenderObject(m_roAxes[i]);
        }
    }

}

void SceneComponent::setVisible(bool visible)
{
    m_roAxes[0]->setVisible(visible);
    m_roAxes[1]->setVisible(visible);
    m_roAxes[2]->setVisible(visible);
    m_roGrid->setVisible(visible);
}

