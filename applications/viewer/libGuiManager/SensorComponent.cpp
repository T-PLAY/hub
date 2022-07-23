#include "SensorComponent.h"

// SensorComponent::SensorComponent()
//{

//}

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

using namespace Ra;
using namespace Ra::Core;
using namespace Ra::Core::Utils;
using namespace Ra::Core::Geometry;
using namespace Ra::Engine;
using namespace Ra::Engine::Rendering;
using namespace Ra::Engine::Data;
using namespace Ra::Engine::Scene;

SensorComponent::SensorComponent(const InputStream& inputStream, Ra::Engine::Scene::Entity* entity)
    : Ra::Engine::Scene::Component(inputStream.getSensorName(), entity)
    , m_inputStream(inputStream)
{
}

void SensorComponent::initialize()
{
    auto plainMaterial = make_shared<PlainMaterial>("Plain Material");
    plainMaterial->m_perVertexColor = true;

    // origin gizmo
    {
        std::vector<std::shared_ptr<Engine::Data::Mesh>> m_meshAxis;
        constexpr Scalar arrowScale = 5_ra;
        constexpr Scalar axisWidth = .1_ra;
        constexpr Scalar arrowFrac = 0_ra;

        std::vector<Color> gizmoColors = {
            Color::Red(), Color::Green(), Color::Blue(), Color::Cyan(), Color::Cyan(), Color::Cyan()
        };

        for (uint i = 0; i < 6; ++i) {
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
            m_roAxes[i] = RenderObject::createRenderObject("Dof6 axis" + std::to_string(i),
                this,
                RenderObjectType::Geometry,
                m_meshAxis[i]);

            m_roAxes[i]->setMaterial(plainMaterial);
            Ra::Core::Transform TLocal = Transform::Identity();
            TLocal.scale(10.0);
            m_roAxes[i]->setLocalTransform(TLocal);
            addRenderObject(m_roAxes[i]);
        }
    }
}
