

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

#include <Eigen/Geometry>
#include <Eigen/src/Geometry/AlignedBox.h>
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

Dof6Component::Dof6Component(
//        const hub::InputSensor& inputStream,
        const hub::SensorSpec & sensorSpec,
                              Ra::Engine::Scene::Entity* entity ) :
//    SensorComponent( inputStream, entity ) {
    SensorComponent( sensorSpec, entity ) {

    assert( m_sensorSpec.getResolutions().size() == 1 );
    m_format = m_sensorSpec.getResolutions().at( 0 ).second;
}

/// This function is called when the component is properly
/// setup, i.e. it has an entity.
void Dof6Component::initialize() {
    SensorComponent::initialize();

    //// setup ////
}

void Dof6Component::update( const hub::Acquisition& acq ) {
    assert( acq.getMeasures().size() == 1 );

    if ( m_format == hub::Format::DOF6 ) {

        const hub::Dof6& dof6 = acq.getMeasures().at( 0 );

        Ra::Core::Vector3 pos( dof6.m_x, dof6.m_y, dof6.m_z );
        Ra::Core::Quaternion orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );

        Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();
        TLocal.translate( pos );
        TLocal.rotate( orientation );

        m_entity->setTransform( TLocal );
    }
    else if ( m_format == hub::Format::MAT4 ) {

        const hub::Measure& measure = acq.getMeasures().at( 0 );
        const auto& mat4 =
            Eigen::Map<Eigen::Matrix<float, 4, 4>>( (float*)measure.m_data ); // Column-major

        m_entity->setTransform( mat4 );
    }
    else { assert( false ); }
}

Aabb Dof6Component::getAabb() const {
    const Aabb aabb = m_entity->computeAabb();
    const auto transform = m_entity->getTransform();

//    aabb.transform( m_entity->getTransform() );
    const Eigen::AlignedBox3f::VectorType rotated_extent_2 = transform.linear().cwiseAbs() * aabb.sizes();
    const Eigen::AlignedBox3f::VectorType rotated_center_2 = transform.linear() * (aabb.max() + aabb.min()) +
        Scalar(2) * transform.translation();
    Eigen::AlignedBox3f::VectorType min, max;
    max = (rotated_center_2 + rotated_extent_2 * 4) / Scalar(2);
    min = (rotated_center_2 - rotated_extent_2 * 4) / Scalar(2);
    return Aabb(min, max);

//    aabb.translate(m_entity->getTransformAsMatrix());
//    return aabb;
}

void Dof6Component::enableTrace( bool enable ) {}

void Dof6Component::enableLive( bool enable ) {}
