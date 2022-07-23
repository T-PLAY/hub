

#include <Dof6Component.hpp>

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
#include <IO/AssimpLoader/AssimpFileLoader.hpp>
#endif

#include <random>

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

Dof6Component::Dof6Component(const InputStream& inputStream, Ra::Engine::Scene::Entity* entity)
    : //    Ra::Engine::Scene::Component( "Dof6 component", entity ) {}
    SensorComponent(inputStream, entity)
{
}

/// This function is called when the component is properly
/// setup, i.e. it has an entity.
void Dof6Component::initialize()
{
    SensorComponent::initialize();

    //    auto blinnPhongMaterial              = make_shared<BlinnPhongMaterial>( "Shaded Material" );
    //    blinnPhongMaterial->m_perVertexColor = true;
    //    blinnPhongMaterial->m_ks = Color::White();
    //    blinnPhongMaterial->m_ns = 100_ra;

    auto lambertianMaterial = make_shared<LambertianMaterial>("Lambertian Material");
    lambertianMaterial->m_perVertexColor = true;

    //// setup ////

    //    // origin ref cube
    //    {
    //        assert(m_ro == nullptr);
    //        std::shared_ptr<Mesh> cube1(new Mesh("Cube"));
    //        const Scalar cubeSide = 50.0;
    //        auto box = Core::Geometry::makeSharpBox(Vector3 { 1_ra, 1_ra, 1_ra } * cubeSide / 2.0, Color::Grey());
    //        cube1->loadGeometry(std::move(box));

    //        m_ro = RenderObject::createRenderObject(
    //            "refCube", this, RenderObjectType::Geometry, cube1, {});
    //        m_ro->setMaterial(lambertianMaterial);
    //        addRenderObject(m_ro);
    //    }
}

void Dof6Component::update(const Stream::Acquisition& acq)
{
    float* translation = (float*)acq.mData; // x, y, z
    float* quaternion = (float*)&acq.mData[12]; // x, y, z, w

    //                Ra::Core::Vector3 pos(10, 10, 10);
    //            -translation[2]);
    Ra::Core::Vector3 pos(translation[0], translation[1], translation[2]);
    //    pos = -pos;

    Ra::Core::Quaternion orientation(quaternion[3], quaternion[0], quaternion[1], quaternion[2]); // w, x, y, z
    //            std::cout << "update pose orientation " << quaternion[0] << ", " << quaternion[1] << ", " << quaternion[2] << ", " << quaternion[3] << std::endl;

    // change to Radium base reference
    Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();
    TLocal.translate(pos);
    TLocal.rotate(orientation);

    m_entity->setTransform(TLocal);
}
