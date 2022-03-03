// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Intel Corporation. All Rights Reserved.

#include <chrono>
#include <iostream> // for cout
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#include <cassert>
#include <hub.h>
#include <stream.h>

// Hello RealSense example demonstrates the basics of connecting to a RealSense device
// and taking advantage of depth data
int main(int argc, char* argv[])
try {
    // Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline p;

    // Configure and start the pipeline
    p.start();

    // Block program until frames arrive
    rs2::frameset frames = p.wait_for_frames();
    const auto start = std::chrono::system_clock::now().time_since_epoch();

    // Try to get a frame of a depth image
    rs2::depth_frame depth = frames.get_depth_frame();
    const auto end = std::chrono::system_clock::now().time_since_epoch();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(start).count() << std::endl;

    // Get the depth frame's dimensions
    const auto width = depth.get_width();
    const auto height = depth.get_height();
    const auto profile = depth.get_profile();
    const auto backendTimestamp = depth.get_frame_metadata(RS2_FRAME_METADATA_BACKEND_TIMESTAMP);
    const auto backendTimeOfArrival = depth.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL);
    assert(depth.get_frame_timestamp_domain() == RS2_TIMESTAMP_DOMAIN_SYSTEM_TIME);

    std::cout << profile.format() << std::endl;
    std::cout << backendTimestamp << std::endl;
    std::cout << backendTimeOfArrival << std::endl;

    std::cout << "duration : " << backendTimeOfArrival - backendTimestamp << std::endl;
    std::cout << "fps : " << 1000.0 / (backendTimeOfArrival - backendTimestamp) << std::endl;

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end).count() << std::endl;

    Stream depthStream(width, height, Stream::Format::Z16);

    return EXIT_SUCCESS;
} catch (const rs2::error& e) {
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
