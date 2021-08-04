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

int threshold_value = 0;
string threshold_text_button = "apply threshold";
bool threshold_bool = false;
#define MAX_BINARY_VALUE 255

void callbackThreshButton( int state, void* userdata);

int main( int argc, char** argv) {
    // cout << getBuildInformation() << endl;
    //--alocate memory
    UMat raw_frame, final_frame;
    string window_name = "Live";

    //-- Init Videocapture
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
    cout << "Press `ctrl + P` to start calibrating" << endl;

    //--- Configure window
    namedWindow(window_name, WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED );
    createButton(threshold_text_button, callbackThreshButton, NULL, QT_CHECKBOX, threshold_bool);

    for (;;) {
        cap.read(raw_frame);
        // Check if successful
        if ( raw_frame.empty() ) {
            cerr << "ERROR! blank frame grabbed...\n";
            break;
        }

        if ( threshold_bool ) {
            final_frame = raw_frame.clone();
        } 
        else {
            final_frame = raw_frame.clone();
        }

        imshow(window_name, final_frame);
        if ( waitKey(5) >= 0 ) {
            break;
        }
    }
    return 0;
}

void callbackThreshButton( int state, void* userdata) {
    // Handle threshold button
    threshold_bool = state;
}