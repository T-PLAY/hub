// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Intel Corporation. All Rights Reserved.

#include <chrono>
#include <iostream> // for cout
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#include <cassert>
#include <stream.h>

// Hello RealSense example demonstrates the basics of connecting to a RealSense device
// and taking advantage of depth data
int main(int argc, char* argv[])

{
    while (true) {
        try {
            OutputStream depthStream("L500 Depth Sensor (Depth)", Stream::Format::Z16, { 640, 480 });
            auto depthAcq = depthStream.acquisition();

            OutputStream rgbStream("L500 RGB Camera", Stream::Format::RGB8, { 1280, 720 });
            auto colorAcq = rgbStream.acquisition();

            OutputStream infraredStream("L500 Depth Sensor (Infrared)", Stream::Format::Y8, { 640, 480 });
            auto irAcq = infraredStream.acquisition();

            //        OutputStream motionStream("L500 Motion Module", Stream::Format::MOTION_XYZ32F, {1});

            // Create a Pipeline - this serves as a top-level API for streaming and processing frames
            rs2::pipeline p;

            // Configure and start the pipeline
            p.start();

            while (true) {
                //                const auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                // Block program until frames arrive
                rs2::frameset frames = p.wait_for_frames();
                //                const auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                // Try to get a frame of a depth image
                rs2::depth_frame depth = frames.get_depth_frame();
                assert(depth.get_data_size() == depthAcq.acquisitionSize);
                depthAcq.backendTimestamp = depth.get_frame_metadata(RS2_FRAME_METADATA_BACKEND_TIMESTAMP);
                depthAcq.backendTimeOfArrival = depth.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL);
                depthAcq.data = (unsigned char*)depth.get_data();
                depthStream << depthAcq;

                rs2::video_frame color = frames.get_color_frame();
                assert(color.get_width() == 1280);
                assert(color.get_height() == 720);
                assert(color.get_data_size() == colorAcq.acquisitionSize);
                colorAcq.backendTimestamp = color.get_frame_metadata(RS2_FRAME_METADATA_BACKEND_TIMESTAMP);
                colorAcq.backendTimeOfArrival = color.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL);
                colorAcq.data = (unsigned char*)color.get_data();
                rgbStream << colorAcq;

                rs2::video_frame ir = frames.get_infrared_frame();
                assert(ir.get_width() == 640);
                assert(ir.get_height() == 480);
                assert(ir.get_data_size() == irAcq.acquisitionSize);
                irAcq.backendTimestamp = ir.get_frame_metadata(RS2_FRAME_METADATA_BACKEND_TIMESTAMP);
                irAcq.backendTimeOfArrival = ir.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL);
                irAcq.data = (unsigned char*)ir.get_data();
                infraredStream << irAcq;

                //            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(start).count() << std::endl;

                // Get the depth frame's dimensions
                //                const auto width = depth.get_width();
                //                const auto height = depth.get_height();
                //                const auto profile = depth.get_profile();
                //                const auto backendTimestamp = depth.get_frame_metadata(RS2_FRAME_METADATA_BACKEND_TIMESTAMP);
                //                const auto backendTimeOfArrival = depth.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL);
                //                const auto frameTimestamp = depth.get_frame_metadata(RS2_FRAME_METADATA_FRAME_TIMESTAMP);

                //            const auto sensorTimestamp = depth.get_frame_metadata(RS2_FRAME_METADATA_SENSOR_TIMESTAMP);

                //                std::cout << "[lidar] lidar backendTimestamp : " << backendTimestamp << std::endl;
                //                std::cout << "[lidar] lidar backendTimeOfArrival : " << backendTimeOfArrival << std::endl;
                //                std::cout << "[lidar] lidar frame timestamp : " << frameTimestamp << std::endl;
                //            std::cout << "lidar sensor timestamp : " << sensorTimestamp << std::endl;
                //            assert(depth.get_frame_timestamp_domain() == RS2_TIMESTAMP_DOMAIN_SYSTEM_TIME);
                //            assert(depth.get_frame_timestamp_domain() == RS2_TIMESTAMP_DOMAIN_GLOBAL_TIME);
                //                assert(depth.get_frame_timestamp_domain() == RS2_TIMESTAMP_DOMAIN_HARDWARE_CLOCK);

                //                std::cout << "[lidar] format : " << profile.format() << std::endl;
                //                std::cout << "[lidar] chrono backendTimestamp : " << start << std::endl;
                //                std::cout << "[lidar] chrono backendTimeOfArrival : " << end << std::endl;

                //            unsigned char * data = (unsigned char*)depth.get_data();

                //            std::cout << "chrono duration : " << backendTimeOfArrival - backendTimestamp << std::endl;
                //            std::cout << "chrono fps : " << 1000.0 / (backendTimeOfArrival - backendTimestamp) << std::endl;

                //            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end).count() << std::endl;

                //    Stream depthStream(width, height, Stream::Format::Z16);

                //            return EXIT_SUCCESS;
            } // while (true)

        } catch (const Socket::exception& e) {
            std::cerr << "[lidar] catch socket exception : " << e.what() << std::endl;
        } catch (const Stream::exception& e) {
            std::cerr << "[lidar] catch stream exception : " << e.what() << std::endl;
        } catch (const rs2::error& e) {
            std::cerr << "[lidar] RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
            return EXIT_FAILURE;
        } catch (const std::exception& e) {
            std::cerr << "[lidar] catch main exception : " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    } // while (true)
}
