#! /bin/bash

#
# Global variables
#
SRC=$PWD

#
# Print menu
#
echo "Calibration script"
echo "=================="
echo
echo "Menu"
echo "----"
echo "1: info/settings camera"
echo "2: get raw view"
echo "3: calibrate setup"

#
# Navigate menu
#

read SLCT

case $SLCT in
    1) # info/settings camera
        echo
        bash set_camera.sh
        ;;
    2) # get raw view
        echo
        FILE=./src/basic/build/basic
        if test -f "$FILE"; then
            bash -c $FILE
        else 
            BUILD_PATH=./src/basic/build
            # If build path does not exist, create it
            if [[ ! -d $BUILD_PATH ]]; then
                echo "Create build directory"
                mkdir $BUILD_PATH
            fi
            # Build basic.cpp
            cd $BUILD_PATH
            cmake -DCMAKE_BUILD_TYPE=Debug ..
            make
            cd $SRC
            bash -c $FILE
        fi
        ;;
    3) # calibrate setup
        echo "Case 3"
        ;;
    *) # default
        echo "This option is not valid"
esac