#pragma once
#include "SensorSpec.hpp"

//#include <array>
#include <numeric>

namespace hub {

static constexpr int s_format2nByte[static_cast<int>( SensorSpec::Format::COUNT )] = {
    0,       // NONE
    2,       // Z16
    2,       // DISPARITY16
    4,       // XYZ32F
    4,       // YUYV
    3,       // RGB8
    3,       // BGR8
    4,       // RGBA8
    4,       // BGRA8
    1,       // Y8
    2,       // Y16
    0,       // RAW10
    2,       // RAW16
    1,       // RAW8
    0,       // UYVY
    0,       // MOTION_RAW
    0,       // MOTION_XYZ32F
    0,       // GPIO_RAW
    0,       // DISPARITY32
    12 + 16, // DOF6
    0,       // Y10BPACK
    4,       // DISTANCE
    0,       // MJPEG
    2,       // Y8I
    3,       // Y12I
    0,       // INZI
    1,       // INVI
    0,       // W10
    2,       // Z16H
    2,       // FG
    2,       // Y411
    64,      // MAT4
};

inline constexpr int SensorSpec::format2nByte(const Format &format) noexcept
{
    return s_format2nByte[static_cast<int>(format)];
}

inline size_t SensorSpec::computeAcquisitionSize( Format format, const Dims& dims ) noexcept {
    return std::accumulate( dims.cbegin(), dims.cend(), 1, std::multiplies<int> {} ) *
            s_format2nByte[static_cast<int>( format )];
}

inline size_t SensorSpec::computeAcquisitionSize( const Resolutions& resolutions ) noexcept {
    size_t size = 0;
    for ( const auto& resolution : resolutions ) {
        size += computeAcquisitionSize( resolution );
    }
    return size;
}

inline size_t SensorSpec::computeAcquisitionSize( const Resolution& resolution ) noexcept {
    const auto& dims   = resolution.first;
    const auto& format = resolution.second;
    return computeAcquisitionSize( format, dims );
}

static constexpr bool format2isInterpolable[static_cast<int>( SensorSpec::Format::COUNT )] = {
    false, // NONE
    false, // Z16
    false, // DISPARITY16
    false, // XYZ32F
    false, // YUYV
    false, // RGB8
    false, // BGR8
    false, // RGBA8
    false, // BGRA8
    false, // Y8
    false, // Y16
    false, // RAW10
    false, // RAW16
    false, // RAW8
    false, // UYVY
    false, // MOTION_RAW
    false, // MOTION_XYZ32F
    false, // GPIO_RAW
    false, // DISPARITY32
    true,  // DOF6
    false, // Y10BPACK
    false, // DISTANCE
    false, // MJPEG
    false, // Y8I
    false, // Y12I
    false, // INZI
    false, // INVI
    false, // W10
    false, // Z16H
    false, // FG
    false, // Y411
    false, // MAT4
};

inline bool SensorSpec::isInterpolable( const Format& format ) noexcept {
    return format2isInterpolable[(int)format];
}


// getters

inline const std::string &SensorSpec::getSensorName() const noexcept
{
    return m_sensorName;
}

const SensorSpec::Resolutions &SensorSpec::getResolutions() const noexcept
{
    return m_resolutions;
}

inline const SensorSpec::MetaData &SensorSpec::getMetaData() const noexcept
{
    return m_metaData;
}

inline SensorSpec::MetaData &SensorSpec::getMetaData() noexcept
{
    return m_metaData;
}

inline size_t SensorSpec::getAcquisitionSize() const noexcept
{
    return m_acquisitionSize;
}

// setters

void SensorSpec::setMetaData(const MetaData &metaData) noexcept
{
    m_metaData = metaData;
}



}
