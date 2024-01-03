// pywrap.cpp
//#include <cassert>
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "SensorSpec.hpp"

#include <client/Streamer.hpp>

#include <Any.hpp>
#include <Eigen/LU>
#include <pybind11/eigen.h>

namespace py         = pybind11;
constexpr auto byref = py::return_value_policy::reference_internal;

typedef Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Matrix8u;

class CustomSensorSpec
{
  public:
    CustomSensorSpec( const std::string& sensorName,
                      const hub::Resolutions& resolutions,
                      const py::dict& dict ) {
        hub::MetaData metaData;
        for ( const auto& item : dict ) {
            const auto& key         = item.first;
            const auto& keyStr      = key.cast<std::string>();
            const auto& value       = item.second;
            const auto& valueDouble = value.cast<double>();
            metaData[keyStr]        = hub::Any( valueDouble );
        }
        m_sensorSpec = hub::sensor::SensorSpec( sensorName, resolutions, metaData );
    }
    hub::sensor::SensorSpec m_sensorSpec;
};

//////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

// a custom vector-class, with one function "mul"
class CustomVector
{
  private:
    Matrix8u m_data;
    hub::Resolution m_resolution;

  public:
    CustomVector( const Matrix8u& data, const hub::Resolution& resolution );

    unsigned char* getData() const;
    uint64_t getSize() const;

    hub::Resolution getResolution() const;
};

// ----------------
// regular C++ code
// ----------------

// class-constructor: store the input "Eigen::VectorXd"

hub::Resolution CustomVector::getResolution() const {
    return m_resolution;
}

CustomVector::CustomVector( const Matrix8u& data, const hub::Resolution& resolution ) :
    m_data( data ), m_resolution( resolution ) {
    assert( m_data.size() == hub::sensor::resolution::computeAcquisitionSize( resolution ) );
}

unsigned char* CustomVector::getData() const {
    return (unsigned char*)m_data.data();
}

uint64_t CustomVector::getSize() const {
    return m_data.size();
}

//// return the custom vector, multiplied by a factor

//// a function that has nothing to do with the class
//// the point is to show how one can return a copy "Eigen::VectorXd"

//////////////////////////////////////////////////////////////////////////////////////////

PYBIND11_MODULE( Hub, m ) {
    m.doc() = "optional module docstring";

    ////    .def("to_string", &MyClass::to_string)
    ////        .def("to_string", [](MyClass & self) -> py::bytes {
    ////            return py::bytes(self.to_string());
    ////        })

    py::class_<CustomSensorSpec>( m, "SensorSpec" )
        .def( py::init<std::string, hub::Resolutions, py::dict>() )
        .def( "to_string", []( CustomSensorSpec& self ) -> py::bytes {
            return py::bytes( self.m_sensorSpec.to_string() );
        } );

    py::class_<hub::sensor::Acquisition>( m, "Acquisition" )
        .def( py::init<long, long>() )
        .def( "addMeasure", []( hub::sensor::Acquisition& self, const CustomVector& customVector ) -> void {
            hub::Measure measure(
                customVector.getData(), customVector.getSize(), customVector.getResolution() );
            self.pushBack( std::move( measure ) );
        } );

    py::class_<CustomVector>( m, "Measure" ).def( py::init<Matrix8u, hub::Resolution>() );

    py::class_<hub::client::Streamer>( m, "Streamer" )
        .def( py::init<std::string, int>() )
        .def( "addStream",
              []( hub::client::Streamer& self,
                  const std::string& streamName,
                  const CustomSensorSpec& sensorSpec ) -> void {
                  self.addStream( streamName, sensorSpec.m_sensorSpec );
              } )
        .def(
            "newAcq",
            []( hub::client::Streamer& self,
                const std::string& streamName,
                const hub::sensor::Acquisition& acq ) -> void { self.newAcquisition( streamName, acq ); } )
        .def( "isConnected", &hub::client::Streamer::isConnected );

    py::enum_<hub::format>( m, "Format" )
        .value(
            "Z16",
            hub::format::Z16,
            "16-bit linear depth values. The depth is meters is equal to depth scale pixel value." )
        .value( "DISPARITY16",
                hub::format::DISPARITY16,
                "16-bit float-point disparity values. Depth->Disparity conversion : Disparity = "
                "Baseline*FocalLength/Depth." )
        .value( "XYZ32F", hub::format::XYZ32F, "32-bit floating point 3D coordinates." )
        .value( "YUYV",
                hub::format::YUYV,
                "32-bit y0, u, y1, v data for every two pixels. Similar to YUV422 but packed in a "
                "different order - https://en.wikipedia.org/wiki/YUV" )
        .value( "RGB8", hub::format::RGB8, "8-bit red, green and blue channels" )
        .value( "BGR8",
                hub::format::BGR8,
                "8-bit blue, green, and red channels -- suitable for OpenCV" )
        .value( "RGBA8",
                hub::format::RGBA8,
                "8-bit red, green and blue channels + constant alpha channel equal to FF" )
        .value( "BGRA8",
                hub::format::BGRA8,
                "8-bit blue, green, and red channels + constant alpha channel equal to FF" )
        .value( "Y8", hub::format::Y8, "8-bit per-pixel grayscale image" )
        .value( "Y16", hub::format::Y16, "16-bit per-pixel grayscale image" )
        .value( "RAW10",
                hub::format::RAW10,
                "Four 10 bits per pixel luminance values packed into a 5-byte macropixel" )
        .value( "RAW16", hub::format::RAW16, "16-bit raw image" )
        .value( "RAW8", hub::format::RAW8, "8-bit raw image" )
        .value( "UYVY",
                hub::format::UYVY,
                "Similar to the standard YUYV pixel format, but packed in a different order" )
        .value( "MOTION_RAW", hub::format::MOTION_RAW, "Raw data from the motion sensor" )
        .value( "MOTION_XYZ32F",
                hub::format::MOTION_XYZ32F,
                "Motion data packed as 3 32-bit float values, for X, Y, and Z axis" )
        .value( "GPIO_RAW",
                hub::format::GPIO_RAW,
                "Raw data from the external sensors hooked to one of the GPIO's" )
        .value( "DISPARITY32",
                hub::format::DISPARITY32,
                "32-bit float-point disparity values. Depth->Disparity conversion : Disparity = "
                "Baseline*FocalLength/Depth" )
        .value( "Dof6",
                hub::format::Dof6,
                "Pose data packed as floats array, containing translation vector (x, y, z), "
                "rotation quaternion (w0, w1, w2, w3 || w, x, y, z)" )
        .value( "Y10BPACK",
                hub::format::Y10BPACK,
                "16-bit per-pixel grayscale image unpacked from 10 bits per pixel packed "
                "([8:8:8:8:2222]) grey-scale image. The data is unpacked to LSB and padded with 6 "
                "zero bits" )
        .value( "DISTANCE", hub::format::DISTANCE, "32-bit float-point depth distance value. " )
        .value(
            "MJPEG",
            hub::format::MJPEG,
            "Bitstream encoding for video in which an image of each frame is encoded as JPEG-DIB" )
        .value( "Y8I", hub::format::Y8I, "8-bit per pixel interleaved. 8-bit left, 8-bit right." )
        .value( "Y12I",
                hub::format::Y12I,
                "12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel is stored in "
                "a 24-bit word in little-endian order." )
        .value( "INZI", hub::format::INZI, "multi-planar Depth 16bit + IR 10bit. " )
        .value( "INVI", hub::format::INVI, "8-bit IR stream. " )
        .value( "W10",
                hub::format::W10,
                "Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 * byte" )
        .value(
            "Z16H", hub::format::Z16H, "Variable-length Huffman-compressed 16-bit depth values." )
        .value( "FG", hub::format::FG, "16-bit per-pixel frame grabber format." )
        .value( "Y411", hub::format::Y411, "12-bit per-pixel." )
        .value( "MAT4", hub::format::Mat4, "Transform matrix 4x4 of float." )
        .value( "USER_DATA", hub::format::USER_DATA, "User data with name and any value" )
        .value( "MESH", hub::format::MESH, "Mesh consist of shapes of vertices with indexes" )
        .value( "POINT",
                hub::format::POINT,
                "Point consist of x, y and z position, rgb color and depth " )
        .value( "DENSITY",
                hub::format::DENSITY,
                "32-bit density values. For MRI, CT scan and US representations." );
}
