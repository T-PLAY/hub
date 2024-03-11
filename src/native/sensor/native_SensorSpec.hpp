/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/12/13

#pragma once

#include "core/Macros.hpp"

namespace hub {

#ifndef HUB_CPP_SOURCE

class MetaData;

namespace sensor {
class SensorSpec;
}
#endif

namespace native {

#ifdef __cplusplus
extern "C"
{
#endif

    ///
    /// \brief createSensorSpec
    /// \param sensorName
    /// \return
    ///
    SRC_API sensor::SensorSpec* createSensorSpec( const char* sensorName );

    ///
    /// \brief freeSensorSpec
    /// \param sensorSpec
    ///
    SRC_API void freeSensorSpec( sensor::SensorSpec* sensorSpec );

    ///
    /// \brief sensorSpec_getSensorName
    /// \param sensorSpec
    /// \param sensorName
    /// \param strLen
    ///
    SRC_API void
    sensorSpec_getSensorName( const sensor::SensorSpec* sensorSpec, char* sensorName, int* strLen );

    ///
    /// \brief sensorSpec_getResolutionsSize
    /// \param sensorSpec
    /// \return
    ///
    SRC_API int sensorSpec_getResolutionsSize( const sensor::SensorSpec* sensorSpec );

    ///
    /// \brief sensorSpec_getResolutionSize
    /// \param sensorSpec
    /// \param iResolution
    /// \return
    ///
    SRC_API int sensorSpec_getResolutionSize( const sensor::SensorSpec* sensorSpec,
                                              int iResolution );

    ///
    /// \brief sensorSpec_getResolutionsStr
    /// \param sensorSpec
    /// \param resolutionsStr
    ///
    SRC_API void sensorSpec_getResolutionsStr( const sensor::SensorSpec* sensorSpec,
                                               char* resolutionsStr );

    ///
    /// \brief sensorSpec_getFormat
    /// \param sensorSpec
    /// \param iResolution
    /// \return
    ///
    SRC_API int sensorSpec_getFormat( const sensor::SensorSpec* sensorSpec, int iResolution );

    ///
    /// \brief sensorSpec_getDimensionsSize
    /// \param sensorSpec
    /// \param iRelosution
    /// \return
    ///
    SRC_API int sensorSpec_getDimensionsSize( const sensor::SensorSpec* sensorSpec,
                                              int iRelosution );

    ///
    /// \brief sensorSpec_getDimension
    /// \param sensorSpec
    /// \param iResolution
    /// \param iDimension
    /// \return
    ///
    SRC_API int sensorSpec_getDimension( const sensor::SensorSpec* sensorSpec, int iResolution, int iDimension);

    ///
    /// \brief sensorSpec_getAcquisitionSize
    /// \param sensorSpec
    /// \return
    ///
    SRC_API int sensorSpec_getAcquisitionSize( const sensor::SensorSpec* sensorSpec );

    ///
    /// \brief sensorSpec_getMetaDataStr
    /// \param sensorSpec
    /// \param metaDataStr
    ///
    SRC_API void sensorSpec_getMetaDataStr( const sensor::SensorSpec* sensorSpec,
                                            char* metaDataStr );

    //SRC_API void to_string( const sensor::SensorSpec* sensorSpec );
    ///
    /// \brief sensorSpec_toString
    /// \param sensorSpec
    /// \param output
    /// \param strLen
    ///
    SRC_API void sensorSpec_toString( const sensor::SensorSpec* sensorSpec, char* output, int* strLen );

    ///
    /// \brief sensorSpec_copy
    /// \param source
    /// \return
    ///
    SRC_API sensor::SensorSpec* sensorSpec_copy( const sensor::SensorSpec* source );

    ///
    /// \brief sensorSpec_getMetaData
    /// \param sensorSpec
    /// \return
    ///
    SRC_API const MetaData* sensorSpec_getMetaData( const sensor::SensorSpec* sensorSpec );

#ifdef __cplusplus
} // end extern "C"
#endif

} // namespace native
} // namespace hub
