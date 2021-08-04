#! /bin/bash

echo "Calibration script"
echo "=================="
echo
echo "Menu"
echo "----"
echo "1: info/settings camera"
echo "2: get raw view"
echo "3: calibrate setup"

read SLCT

case $SLCT in
    1)
        echo
        bash set_camera.sh
        ;;
    2)
        echo "Case 2"
        ;;
    3)
        echo "Case 3"
        ;;
    *)
        echo "This option is not valid"
esac