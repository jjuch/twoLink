#include <iostream>
#include <stdio.h>
#include <string>

#include <libv4l2.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

int main( int, char** ) {
    //-- alocate memory
    UMat raw_frame;
    string windowName = "Live";

    //--- INIT VideoCapture
    VideoCapture cap;
    int deviceID = -1;
    int apiID = cv::CAP_V4L2;
    
    // open camera
    int success = cap.open( deviceID, apiID );

    // Check if successful
    if ( !cap.isOpened() ) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    //--- Grab and write loop
    cout << "Start grabbing" << endl;
    cout << "Quit by pressing `Q`" << endl;

    //-- Set window properties
    /*namedWindow(windowName, WINDOW_NORMAL);
    setWindowProperty(windowName, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);*/

    for (;;) {
        cap.read(raw_frame);
        // Check if successful
        if ( raw_frame.empty() ) {
            cerr << "ERROR! blank frame grabbed...\n";
            break;
        }

        imshow(windowName, raw_frame);
        
        if ( waitKey(5) >= 0 ) {
            break;
        }
    }
}