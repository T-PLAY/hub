

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

//#include <Eigen/Dense>

#ifdef IO_USE_ASSIMP
#    include <IO/AssimpLoader/AssimpFileLoader.hpp>
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

Dof6Component::Dof6Component( const hub::InputSensor& inputStream,
                              Ra::Engine::Scene::Entity* entity ) :
    //    Ra::Engine::Scene::Component( "Dof6 component", entity ) {}
    SensorComponent( inputStream, entity ) {

    assert( m_inputSensor.m_spec.m_resolutions.size() == 1 );
    m_format = m_inputSensor.m_spec.m_resolutions.at( 0 ).second;
}

/// This function is called when the component is properly
/// setup, i.e. it has an entity.
void Dof6Component::initialize() {
    SensorComponent::initialize();

    //    auto blinnPhongMaterial              = make_shared<BlinnPhongMaterial>( "Shaded Material"
    //    ); blinnPhongMaterial->m_perVertexColor = true; blinnPhongMaterial->m_ks = Color::White();
    //    blinnPhongMaterial->m_ns = 100_ra;

    //    auto lambertianMaterial              = make_shared<LambertianMaterial>( "Lambertian
    //    Material" ); lambertianMaterial->m_perVertexColor = true;

    //// setup ////

    //    // origin ref cube
    //    {
    //        assert(m_ro == nullptr);
    //        std::shared_ptr<Mesh> cube1(new Mesh("Cube"));
    //        const Scalar cubeSide = 50.0;
    //        auto box = Core::Geometry::makeSharpBox(Vector3 { 1_ra, 1_ra, 1_ra } * cubeSide / 2.0,
    //        Color::Grey()); cube1->loadGeometry(std::move(box));

    //        m_ro = RenderObject::createRenderObject(
    //            "refCube", this, RenderObjectType::Geometry, cube1, {});
    //        m_ro->setMaterial(lambertianMaterial);
    //        addRenderObject(m_ro);
    //    }

//    if ( m_format == hub::SensorSpec::Format::DOF6 ) {
//        for ( uint i = 0; i < 3; ++i ) {

//            Ra::Core::Transform TLocal = Transform::Identity();
//            TLocal.scale( 100.0 );
//            m_roAxes[i]->setLocalTransform( TLocal );
////            addRenderObject( m_roAxes[i] );
//        }
//    }
}

void Dof6Component::update( const hub::Acquisition& acq ) {
    //    float* translation = (float*)acq.m_data; // x, y, z
    //    float* quaternion = (float*)&acq.m_data[12]; // x, y, z, w
    assert( acq.getMeasures().size() == 1 );
    //    const auto& format = m_inputSensor.m_spec.m_resolutions.at( 0 ).second;

    if ( m_format == hub::SensorSpec::Format::DOF6 ) {

        const hub::Dof6& dof6 = acq.getMeasures().at( 0 );

        Ra::Core::Vector3 pos( dof6.m_x, dof6.m_y, dof6.m_z );
        Ra::Core::Quaternion orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );

        Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();
        TLocal.translate( pos );
        TLocal.rotate( orientation );

        m_entity->setTransform( TLocal );
    }
    else if ( m_format == hub::SensorSpec::Format::MAT4 ) {

        const hub::Measure& measure = acq.getMeasures().at( 0 );
        //        auto mat4 = Eigen::Map<Eigen::Matrix<float, 4, 4,
        //        Eigen::RowMajor>>((float*)measure.m_data); // Row-major
        const auto& mat4 =
            Eigen::Map<Eigen::Matrix<float, 4, 4>>( (float*)measure.m_data ); // Column-major
        //        std::cout << mat4 << std::endl;

        m_entity->setTransform( mat4 );
        //        for (int i = 0; i <3; ++i) {
        //            m_roAxes[i]->setLocalTransform(mat4);
        //        }
    }
    else { assert( false ); }
}

Aabb Dof6Component::getAabb() const
{
    Aabb aabb = m_entity->computeAabb();
    aabb.transform(m_entity->getTransform());
//    return m_entity->computeAabb();
//    return Aabb();
    return aabb;
}
