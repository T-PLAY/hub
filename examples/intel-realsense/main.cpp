// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Intel Corporation. All Rights Reserved.

#include <chrono>
#include <iostream> // for cout
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#include <cassert>

#include <OutputSensor.hpp>

#define DEPTH_STREAM
#define RGB_STREAM
#define INFRARED_STREAM

// Hello RealSense example demonstrates the basics of connecting to a RealSense device
// and taking advantage of depth data
int main(int argc, char* argv[])

{
    while (true) {
        try {
#ifdef DEPTH_STREAM
            hub::OutputSensor depthStream(
                { "L500 Depth Sensor (Depth)", { { { 640, 480 }, hub::Format::Z16 } } },
                hub::io::OutputStream("L500 Depth Sensor (Depth)"));
#endif

#ifdef RGB_STREAM
            //                hub::OutputSensor rgbStream("L500 RGB Camera", hub::Format::RGB8, { 640, 480 });
            hub::OutputSensor rgbStream(
                { "L500 RGB Camera", { { { 640, 480 }, hub::Format::RGB8 } } },
                hub::io::OutputStream("L500 RGB Camera"));
#endif

#ifdef INFRARED_STREAM
            //                hub::OutputSensor infraredStream(
            //                    "L500 Depth Sensor (Infrared)", hub::Format::Y8, { 640, 480 });
            hub::OutputSensor infraredStream(
                { "L500 Depth Sensor (Infrared)", { { { 640, 480 }, hub::Format::Y8 } } },
                hub::io::OutputStream("L500 Depth Sensor (Infrared)"));
#endif

            // Create a Pipeline - this serves as a top-level API for streaming and processing
            // frames
            rs2::pipeline p;

            rs2::config cfg;
#ifdef DEPTH_STREAM
            cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
#endif
#ifdef RGB_STREAM
            cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, 30);
#endif
#ifdef INFRARED_STREAM
            cfg.enable_stream(RS2_STREAM_INFRARED, 640, 480, RS2_FORMAT_Y8, 30);
#endif

            // Configure and start the pipeline
            p.start(cfg);

            int iFrame = 0;
            while (true) {
                // Block program until frames arrive
                rs2::frameset frames = p.wait_for_frames(2000);
                std::cout << "receive frame " << iFrame << std::endl;

#ifdef DEPTH_STREAM
                // Try to get a frame of a depth image
                rs2::depth_frame depth = frames.get_depth_frame();
                const size_t& depthSize = depth.get_data_size();
                assert(depthSize == depthStream.m_spec.getAcquisitionSize());
                depthStream << (hub::Acquisition { depth.get_frame_metadata(RS2_FRAME_METADATA_BACKEND_TIMESTAMP),
                                    depth.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL) }
                                << hub::Measure { (unsigned char*)depth.get_data(), depthSize, {{640, 480}, hub::Format::Z16} });
#endif

#ifdef RGB_STREAM
                rs2::video_frame color = frames.get_color_frame();
                const size_t colorSize = color.get_data_size();
                assert(colorSize == rgbStream.m_spec.getAcquisitionSize());
                rgbStream << (hub::Acquisition {
                                  color.get_frame_metadata(RS2_FRAME_METADATA_BACKEND_TIMESTAMP),
                                  color.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL) }
                              << hub::Measure { (unsigned char*)color.get_data(), colorSize, {{640, 480}, hub::Format::RGB8} });
#endif

#ifdef INFRARED_STREAM
                rs2::video_frame ir = frames.get_infrared_frame();
                const size_t infraredSize = ir.get_data_size();
                assert(infraredSize == infraredStream.m_spec.getAcquisitionSize());
                infraredStream << (hub::Acquisition {
                                       ir.get_frame_metadata(RS2_FRAME_METADATA_BACKEND_TIMESTAMP),
                                       ir.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL) }
                    << hub::Measure { (unsigned char*)ir.get_data(), infraredSize, {{640, 480}, hub::Format::Y8}});
#endif

                ++iFrame;
            } // while (true)
        } catch (const hub::net::Socket::exception& e) {
            std::cerr << "[lidar] catch socket exception : " << e.what() << std::endl;
//        } catch (const hub::Stream::exception& e) {
//            std::cerr << "[lidar] catch stream exception : " << e.what() << std::endl;
        } catch (const rs2::error& e) {
            std::cerr << "[lidar] RealSense error calling " << e.get_failed_function() << "("
                      << e.get_failed_args() << "):\n    " << e.what() << std::endl;
            return EXIT_FAILURE;
        } catch (const std::exception& e) {
            std::cerr << "[lidar] catch std exception : " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    } // while (true)
}
