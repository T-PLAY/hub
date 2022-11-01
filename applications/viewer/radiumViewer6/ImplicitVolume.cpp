#include "ImplicitVolume.h"

#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

// ImplicitVolume::ImplicitVolume()
//{
// }

void ImplicitVolume::cube( Ra::Engine::RadiumEngine& engine ) {

    //! [Creating the Volume]
    auto density = new Ra::Core::Geometry::VolumeGrid();
    int sx       = 100;
    int sy       = 100;
    int sz       = 100;

    density->setSize( Ra::Core::Vector3i( sx, sy, sz ) );
    auto& data = density->data();
    // Radial density with value 1 at center
    auto densityField = [sx, sy, sz]( int i, int j, int k, int r ) { return 1.0; };
    for ( int i = 0; i < sx; ++i ) {
        for ( int j = 0; j < sy; ++j ) {
            for ( int k = 0; k < sz; ++k ) {
                data[i + sx * ( j + sy * k )] = densityField( i, j, k, 50 );
            }
        }
    }

    auto volume    = new Ra::Core::Asset::VolumeData( "DemoVolume" );
    volume->volume = density;
    Scalar maxDim  = std::max( std::max( sx, sy ), sz );
    Ra::Core::Vector3 p0( 0, 0, 0 );
    Ra::Core::Vector3 p1( sx, sy, sz );
    volume->boundingBox    = Ra::Core::Aabb( p0, p1 );
    volume->densityToModel = Ra::Core::Transform::Identity();
    //! [Creating the Volume]

    //! [Create the engine entity for the Volume]
    auto e = engine.getEntityManager()->createEntity( "Volume demo" );
    //! [Create the engine entity for the Volume]

    //! [Create a geometry component with the Volume]
    auto c = new Ra::Engine::Scene::VolumeComponent( "Volume Demo", e, volume );
    //! [Create a geometry component with the Volume]

    //! [Register the entity/component association to the geometry system ]
    auto geometrySystem = engine.getSystem( "GeometrySystem" );
    geometrySystem->addComponent( e, c );

    //! [Register the entity/component association to the geometry system ]
    //! [Creating the cube]
    //! [Creating the cube]
}
