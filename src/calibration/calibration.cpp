#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>

#include <libv4l2.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

#define MAX_BINARY_VALUE 255
#define MAX_RESOLUTION_VALUE 10 // divide by 10
#define MAX_BLUR_VALUE 50
int threshold_value = 125;
float resolution_value = 1.0;
int blur_value = 25; // kernel size
int static_point_x = 0;
int static_point_y = 0;
const string threshold_text_button = "apply threshold";
const string threshold_text_trackbar = "Threshold value";
const string blur_text_trackbar = "Kernel size blur";
const string resolution_text_trackbar = "Resolution size";
const string save_text_button = "save";

bool threshold_bool = false;


void callbackThreshButton( int state, void* user_data );
void callbackResolutionTrackbar( int value, void* user_data );
void callbackBlurTrackbar( int value, void* user_data );
void callbackThresholdTrackbar( int value, void* user_data );
void indicateStaticPoint( int action, int x, int y, int flags, void* ptr_dot_frame);
void applyThresholding( UMat &src, UMat &dst );
void callbackSaveButton( int state, void* user_data );

typedef UMat UMat_ptr;

int main( int argc, char** argv) {
    // cout << getBuildInformation() << endl;
    //--alocate memory
    UMat raw_frame, final_frame, resulting_frame;
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

    // create frame for static_dot
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    UMat_ptr dot_frame(frame_height, frame_width, CV_8U, Scalar(0, 0, 0));


    //--- Grab and write loop
    cout << "Start grabbing" << endl;
    cout << "Quit by pressing `Q`" << endl;
    cout << "Press `ctrl + P` to start calibrating" << endl;

    //--- Configure window
    namedWindow(window_name, WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED );
    createButton(threshold_text_button, callbackThreshButton, NULL, QT_CHECKBOX, threshold_bool);
    createTrackbar(resolution_text_trackbar, window_name, NULL, MAX_RESOLUTION_VALUE, callbackResolutionTrackbar, NULL);
    createTrackbar(blur_text_trackbar, window_name, NULL, MAX_BLUR_VALUE, callbackBlurTrackbar, NULL);
    createTrackbar(threshold_text_trackbar, window_name, NULL, MAX_BINARY_VALUE, callbackThresholdTrackbar, NULL);
    setMouseCallback(window_name, indicateStaticPoint, &dot_frame);
    createButton(save_text_button, callbackSaveButton, NULL);

    for (;;) {
        cap.read(raw_frame);
        // Check if successful
        if ( raw_frame.empty() ) {
            cerr << "ERROR! blank frame grabbed...\n";
            break;
        }

        //-- Decrease resolution
        UMat raw_frame_temp = raw_frame.clone();
        // cout << "resize param: " << resolution_value << endl;
        resize(raw_frame_temp, raw_frame, Size(), resolution_value, resolution_value, INTER_LINEAR);

        // update dot_frame size
        if ( raw_frame.size() != dot_frame.size() ) {
            UMat dot_frame_clone = dot_frame.clone();
            resize(dot_frame_clone, dot_frame, raw_frame.size(), 0.0, 0.0, INTER_LINEAR);
        }


        if ( threshold_bool ) {
            applyThresholding( raw_frame, final_frame );
        } 
        else {
            cvtColor(raw_frame, final_frame, COLOR_BGR2GRAY);
            final_frame.convertTo(final_frame, CV_8U);
            // final_frame = raw_frame.clone();
        }
        // cout << "size: " << dot_frame.size() << endl;
        addWeighted(final_frame, 0.5, dot_frame, 0.5, 0, resulting_frame);
        // resulting_frame = final_frame.clone();

        //-- show image
        imshow(window_name, resulting_frame);
        if ( waitKey(20) >= 0 ) {
            break;
        }
    }
    return 0;
}

void callbackThreshButton( int state, void* user_data) {
    // Handle threshold button
    threshold_bool = state;
}

void callbackResolutionTrackbar( int value, void* user_data ) {
    // Handle resolution trackbar - is times 10, because trackbar cannot handle floating values
    if ( value > 0) {
        // scaling factor as trackbar cannot handle floats
        resolution_value = value / 10.0;
    } else {
        // resize cannot handle a 0 resize
        resolution_value = 0.1;
    }
}

void callbackBlurTrackbar( int value, void* user_data ) {
    // Handle blur trackbar - should be positive and odd
    if (value % 2 == 0) {
        blur_value = value + 1;
    } else {
        blur_value = value;
    }
}

void callbackThresholdTrackbar( int value, void* user_data ) {
    // Handle threshold trackbar
    threshold_value = value;
}

void indicateStaticPoint( int action, int x, int y, int flags, void* ptr_dot_frame) {
    if ( action == EVENT_LBUTTONDOWN ) {
        // assign position to global variables
        static_point_x = x;
        static_point_y = y;

        // cast void to UMat pointer
        UMat_ptr *frame = static_cast<UMat_ptr *>(ptr_dot_frame);

        // reset frame 
        *frame = UMat::zeros(frame->rows, frame->cols, CV_8U);

        // draw circle
        Point center = Point(x, y);
        circle(*frame, center, ceil(10*resolution_value), Scalar(255), FILLED, LINE_8);
        circle(*frame, center, ceil(5*resolution_value), Scalar(0), FILLED, LINE_8); // More control over the hole size
    }
}

void callbackSaveButton( int state, void* user_data ) {
    cout << "Saving..." << state << endl;
    
    cout << "Saving succesfull" << endl;
}

void applyThresholding( UMat &src, UMat &dst) {
    UMat blur, bit;

    // use threshold
    GaussianBlur(src, blur, Size(blur_value, blur_value), 10, 10);
    threshold(blur, bit, threshold_value, MAX_BINARY_VALUE, THRESH_BINARY);

    // convert to grayscale 8-bit
    cvtColor(bit, dst, COLOR_BGR2GRAY);
    dst.convertTo(dst, CV_8U);
}