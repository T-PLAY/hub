
#include <DicomLoader/DicomLoader.hpp>
#include <DicomLoader/DicomUtils.hpp>

#include <Core/Asset/FileData.hpp>
#include <Core/Geometry/Volume.hpp>
#include <Core/Utils/Log.hpp>

#include <Engine/Scene/SystemDisplay.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>

namespace Ra {
namespace IO {

using namespace Ra::Core;
using namespace Ra::Core::Utils;

static const std::string volFileExtension { "vol" };
static const std::string dicomFileExtension { "dcm" };

DicomLoader::DicomLoader()  = default;
DicomLoader::~DicomLoader() = default;

std::vector<std::string> DicomLoader::getFileExtensions() const {
    return { { "*." + volFileExtension }, { "*." + dicomFileExtension } };
}

bool DicomLoader::handleFileExtension( const std::string& extension ) const {
    return ( extension.compare( volFileExtension ) == 0 ) ||
           ( extension.compare( dicomFileExtension ) == 0 );
}

Ra::Core::Utils::Color readColor( std::ifstream& input, std::string& name ) {
    char beg, end;
    Scalar r, g, b;
    input >> name >> beg >> r >> g >> b >> end;
    if ( beg == '[' && end == ']' ) { return Ra::Core::Utils::Color( r, g, b ); }
    return Ra::Core::Utils::Color( 0, 0, 0 );
}

bool checkExpected( const std::string& expected, const std::string& found ) {
    if ( expected != found ) {
        LOG( logWARNING ) << "\tVolumeLoader : unexpected volume attribute " << found << " : "
                          << expected << " was expected";
        return false;
    }
    return true;
}

Ra::Core::Asset::FileData* DicomLoader::loadVolFile( const std::string& filename ) {
    LOG( logINFO ) << "DicomLoader : loading vol (pbrt based) file " << filename;
    std::ifstream input( filename );
    if ( input.is_open() ) {
        auto fileData = new Ra::Core::Asset::FileData( filename );
        std::string attribname;
        auto sigma_a = readColor( input, attribname );
        if ( !checkExpected( "sigma_a", attribname ) ) { return nullptr; }
        auto sigma_s = readColor( input, attribname );
        if ( !checkExpected( "sigma_s", attribname ) ) { return nullptr; }

        char beg, end;
        int sx, sy, sz;

        input >> attribname >> beg >> sx >> sy >> sz >> end;
        if ( !checkExpected( "size", attribname ) ) { return nullptr; }

        input >> attribname >> beg;
        if ( !checkExpected( "density", attribname ) ) { return nullptr; }
        if ( beg != '[' ) {
            LOG( logWARNING ) << "\tVolumeLoader : unexpected start of density gri delimiter "
                              << beg;
            return nullptr;
        }
        LOG( logINFO ) << "\tVolumeLoader : reading a volume of size " << sx << "x" << sy << "x"
                       << sz;

        Ra::Core::Vector3 voxelSize { 1_ra, 1_ra, 1_ra };
        auto density = new Geometry::VolumeGrid();
        density->setSize( Vector3i( sx, sy, sz ) );
        density->setBinSize( voxelSize );
        std::generate( density->data().begin(), density->data().end(), [&input]() {
            Scalar v;
            input >> v;
            return v;
        } );

        input >> end;
        if ( end != ']' ) {
            LOG( logWARNING ) << "\tVolumeLoader : unexpected end of density grid delimiter "
                              << end;
            return nullptr;
        }
        LOG( logINFO ) << "\tVolumeLoader : done reading";

        auto volume = new Asset::VolumeData( filename.substr( filename.find_last_of( '/' ) + 1 ) );
        volume->volume  = density;
        volume->sigma_a = sigma_a;
        volume->sigma_s = sigma_s;
        Scalar maxDim   = std::max( std::max( sx, sy ), sz );
        Ra::Core::Vector3 p0( 0, 0, 0 );
        Ra::Core::Vector3 p1( sx, sy, sz );
        volume->boundingBox    = Aabb( p0, p1 );
        volume->densityToModel = Transform::Identity(); // Eigen::Scaling( 1_ra / maxDim );
        volume->modelToWorld   = Eigen::Scaling( 1_ra / maxDim ); // Transform::Identity();
        fileData->m_volumeData.push_back( std::unique_ptr<Ra::Core::Asset::VolumeData>( volume ) );
        return fileData;
    }
    LOG( logWARNING ) << "DicomLoader : unable to open file " << filename;
    return nullptr;
}

Ra::Core::Asset::FileData* DicomLoader::loadDicomFile( const std::string& filename ) {
    std::cout << "[DicomLoader] loadDicomFile '" << filename << "'" << std::endl;
    using namespace DICOM;

    unsigned int imageWidth, imageHeight, nImages, bytePerVoxel;
    float pixelSpacingWidth, pixelSpacingHeight, sliceThickness;

    auto volumeData = readDicomVolume( filename.c_str(),
                                       &imageWidth,
                                       &imageHeight,
                                       &nImages,
                                       &bytePerVoxel,
                                       &pixelSpacingWidth,
                                       &pixelSpacingHeight,
                                       &sliceThickness );
    if ( volumeData ) {
        {
            LOG( logINFO ) << "DicomLoader : \n\t (DCMTK library) file " << filename
                           << " \n\twidth = " << imageWidth << " \n\theight = " << imageHeight
                           << " \n\tdepth = " << nImages << " \n\tbyte per voxel = " << bytePerVoxel
                           << " \n\tscalex = " << pixelSpacingWidth
                           << " \n\tscaley = " << pixelSpacingHeight
                           << " \n\tscalez = " << sliceThickness << std::endl;
        }
        auto fillRadiumVolume = []( auto container, auto densityData ) {
            std::generate( container->data().begin(), container->data().end(), [&densityData]() {
                auto d = *densityData++;
                return Scalar( d ) / Scalar(std::numeric_limits<decltype( d )>::max());
            } );
        };
        Ra::Core::Vector3 binSize {
            Scalar( pixelSpacingWidth ), Scalar( pixelSpacingHeight ), Scalar( sliceThickness ) };
        Ra::Core::Vector3i gridSize { int( imageWidth ), int( imageHeight ), int( nImages ) };
        auto density = new Geometry::VolumeGrid();
        density->setSize( gridSize );
        density->setBinSize( binSize );

        assert( bytePerVoxel == 2 );
        //        bytePerVoxel = 1;
        switch ( bytePerVoxel ) {
        case 1: {
            fillRadiumVolume( density, volumeData );
            break;
        }
        case 2: {
            fillRadiumVolume( density, (uint16_t*)( volumeData ) );
            break;
        }
        case 4: {
            fillRadiumVolume( density, (uint*)( volumeData ) );
            break;
        }
        default:
            LOG( logERROR ) << "DicomLoader : unsupported number of componenets : " << bytePerVoxel;
        }
        //        free( volumeData );
        delete[] volumeData;

        LOG( logINFO ) << "\tVolumeLoader : done reading";

        Ra::Core::Transform TRadium = Ra::Core::Transform::Identity();
        TRadium.rotate(
            Eigen::AngleAxis( -0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3( 0.0, 1.0, 0.0 ) ) );
        TRadium.rotate(
            Eigen::AngleAxis( -0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3( 1.0, 0.0, 0.0 ) ) );

        auto volume = new Asset::VolumeData( filename.substr( filename.find_last_of( '/' ) + 1 ) );
        volume->volume         = density;
        Scalar maxDim          = std::max( std::max( imageWidth, imageHeight ), nImages );
        Ra::Core::Vector3 p0   = Vector3::Zero();
        Ra::Core::Vector3 p1   = gridSize.cast<Scalar>().cwiseProduct( binSize );
        volume->boundingBox    = Aabb( p0, p1 );
        volume->densityToModel = Eigen::Scaling( binSize );

        volume->modelToWorld = TRadium;
        //        volume->modelToWorld   = Eigen::Scaling( 0.1_ra );
        //        volume->modelToWorld   = Eigen::Scaling( 1_ra / maxDim ) * Translation( p1 * -0.5
        //        );

        std::cout << "p1 = " << p1.x() << ", " << p1.y() << ", " << p1.z() << std::endl;

        Aabb aabb( p0, TRadium * p1 );
        RA_DISPLAY_AABB( aabb, Color::Blue() );

        auto fileData = new Ra::Core::Asset::FileData( filename );
        fileData->m_volumeData.push_back( std::unique_ptr<Ra::Core::Asset::VolumeData>( volume ) );
        return fileData;
    }
    LOG( logWARNING ) << "DicomLoader : unable to open file " << filename;
    return nullptr;
}

Ra::Core::Asset::FileData* DicomLoader::loadFile( const std::string& filename ) {
    std::string extension = filename.substr( filename.find_last_of( '.' ) + 1 );
    if ( extension.compare( volFileExtension ) == 0 ) { return loadVolFile( filename ); }
    else if ( extension.compare( dicomFileExtension ) == 0 ) { return loadDicomFile( filename ); }
    LOG( logWARNING ) << "DicomLoader : unsupported file format : " << filename;
    return nullptr;
}

std::string DicomLoader::name() const {
    return "DicomLoader (pbrt experimental, pvm)";
}

} // namespace IO
} // namespace Ra
