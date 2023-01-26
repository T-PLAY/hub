
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>
#include <thread>

#include <OutputSensor.hpp>

/// \file

int main( int, char** ) {
    cv::Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;           // 0 = open default camera
    int apiID    = cv::CAP_ANY; // 0 = autodetect default API
    // open selected camera using selected API
    cap.open( deviceID, apiID );
    // check if we succeeded
    if ( !cap.isOpened() ) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    std::cout << "Able to open camera\n";

    const int width  = cap.get( cv::CAP_PROP_FRAME_WIDTH );
    const int height = cap.get( cv::CAP_PROP_FRAME_HEIGHT );
    //    OutputStream outputStream( "webcam", Stream::Format::BGR8, { width, height } );
    hub::SensorSpec sensorSpec( "Webcam", { { { width, height }, hub::Format::BGR8 } } );
    hub::OutputSensor outputSensor( sensorSpec, hub::io::OutputStream( "Webcam" ) );

    //--- GRAB AND WRITE LOOP
    std::cout << "Start grabbing" << std::endl << "Press any key to terminate" << std::endl;
    bool stopped       = false;
    std::thread thread = std::thread( [&]() {
        while ( !stopped ) {
            // wait for a new frame from camera and store it into 'frame'
            auto start = std::chrono::duration_cast<std::chrono::microseconds>(
                             std::chrono::system_clock::now().time_since_epoch() )
                             .count();
            cap >> frame;
            //        std::cout << "New frame" << std::endl;

            auto end = std::chrono::duration_cast<std::chrono::microseconds>(
                           std::chrono::system_clock::now().time_since_epoch() )
                           .count();

            //            cv::Mat grey;
            //            cv::cvtColor( frame, grey, cv::COLOR_BGR2GRAY );
            //            cv::Mat rgb;
            //            cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
            //        outputStream << Stream::Acquisition( start, end, frame.data, 3 * width *
            //        height );
            outputSensor
                << ( hub::Acquisition { start, end } << hub::Measure {
                         (const unsigned char*)frame.data, // copy data pointer
                         //                              << hub::Measure { (const unsigned
                         //                              char*)rgb.data, // copy data pointer
                         ( uint64_t )( 3 * width * height ),
                         { { width, height }, hub::Format::BGR8 } } );

            // check if we succeeded
            if ( frame.empty() ) {
                std::cerr << "ERROR! blank frame grabbed\n";
                break;
            }
            // show live and wait for a key with timeout long enough to show images
            // esc
            //        if ((char)27 == cv::waitKey(10)) break;
            //        if (cv::waitKey(20) > 0) break;
            //        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    } );

    getchar();
    stopped = true;
    thread.join();

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
