
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>

#include <stream.h>

int main(int, char**)
{
    cv::Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0; // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    const int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    const int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
//    const auto format = cap.get(cv::CAP_PROP_FORMAT);
//    OutputStream outputStream("webcam", Stream::Format::BGR8, {width, height});
    OutputStream outputStream("webcam", Stream::Format::Y8, {width, height});
    auto acq = outputStream.acquisition();

    //--- GRAB AND WRITE LOOP
    std::cout << "Start grabbing" << std::endl
              << "Press any key to terminate" << std::endl;
    for (;;) {
        // wait for a new frame from camera and store it into 'frame'
//        cap.read(frame);
        acq.start();
        cap >> frame;
        acq.end();

        cv::Mat grey;
        cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
//        unsigned char * data = frame.data;
//        auto size = frame.size;
//        acq.data = frame.data;
        acq.data = grey.data;
//        assert(size == outputStream.getAcquisitionSize());
        outputStream << acq;

        // check if we succeeded
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
//        imshow("Live", frame);
//        if (cv::waitKey(5) >= 0)
//            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
