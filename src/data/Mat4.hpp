/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/03/14
	
#pragma once

// #include <string>

// #include "Measure.hpp"
// #include "sensor/Format.hpp"

namespace hub {
namespace data {

// using Mat4 = sensor::format::Mat4;

}
} // namespace hub

// ///
// /// \brief The Mat4 class
// /// represents a transform matrix (row-major).
// ///
// class SRC_API Mat4 : public Measure
// {

//   public:
//     ///
//     /// \brief Mat4
//     /// \param measure
//     ///
//     explicit Mat4( const Measure& measure );

//     ///
//     /// \brief Mat4
//     /// \param mat4
//     ///
//     Mat4( const Mat4& mat4 );

//     ///
//     /// \brief Mat4
//     /// is the 4x4 transformation matrix (row-major).
//     /// \param array
//     /// of 4x4 float (size = 4x4x4 = 64 bytes)
//     ///
//     explicit Mat4( float* array );

//     ///
//     /// \brief Mat4
//     /// \param array
//     ///
//     explicit Mat4( const float* array );

//     // clang-format off
//         ///
//         /// \brief Mat4
//         /// \param r0c0
//         /// \param r0c1
//         /// \param r0c2
//         /// \param r0c3
//         /// \param r1c0
//         /// \param r1c1
//         /// \param r1c2
//         /// \param r1c3
//         /// \param r2c0
//         /// \param r2c1
//         /// \param r2c2
//         /// \param r2c3
//         /// \param r3c0
//         /// \param r3c1
//         /// \param r3c2
//         /// \param r3c3
//         ///
//     explicit Mat4(
//         float r0c0 = 1.0, float r0c1 = 0.0, float r0c2 = 0.0, float r0c3 = 0.0,
//         float r1c0 = 0.0, float r1c1 = 1.0, float r1c2 = 0.0, float r1c3 = 0.0,
//         float r2c0 = 0.0, float r2c1 = 0.0, float r2c2 = 1.0, float r2c3 = 0.0,
//         float r3c0 = 0.0, float r3c1 = 0.0, float r3c2 = 0.0, float r3c3 = 1.0
//     );
//     // clang-format on

//     ///
//     /// \brief to_string
//     /// \return
//     ///
//     std::string to_string() const;

//     ///
//     /// \brief operator <<
//     /// \param os
//     /// \param mat4
//     /// \return
//     ///
//     SRC_API friend std::ostream& operator<<( std::ostream& os, const Mat4& mat4 );

//     ///
//     /// \brief operator []
//     /// \param idx
//     /// \return
//     ///
//     const float* operator[]( int idx ) const;
// };

// } // namespace data
// } // namespace hub
