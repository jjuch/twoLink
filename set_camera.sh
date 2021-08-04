#!/bin/bash
echo "Info about the CAM-MIPI-OV9281"
echo "=============================="
echo
echo "Menu"
echo "----"
echo "0: get camera mode"
echo "1: set camera mode"
echo "2: do a fps test"

read SLCT
if [[ $SLCT -eq 0 ]]
then
    echo "-> camera mode: "
    cat /sys/module/vc_mipi_ov9281/parameters/sensor_mode
elif [[ $SLCT -eq 1 ]]
then
    echo "Change parameter 'vc_mipi_ov9281.sensor_mode' in cmdline.txt file"
    sudo vim /boot/cmdline.txt
    echo "Reboot now?(y/n):"
    read KB_INPUT
    case $KB_INPUT in
        'y'|'Y')
            echo "reboot..."
            sudo reboot
        ;;
        *)
            echo "cancel."
        ;;
    esac
elif [[ $SLCT -eq 2 ]]
then
    SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
    cd $SCRIPT_DIR/cam-mipiov9281/tools
    v4l2-ctl --stream-mmap --stream-count=600 -d /dev/video0 --stream-to=/dev/null
    cd $SCRIPT_DIR
else
    echo "This mode does not exist"
fi