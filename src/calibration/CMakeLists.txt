#-----------------------------------------------------
# Root CMake file for the basic camera function
#
#-----------------------------------------------------

# set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING 
        "Choose the type of build, options are: Debug Release 
        RelWithDebInfo MinSizeRel."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)

cmake_minimum_required(VERSION 3.16)

# set project name
project(Calibration)

#
# include opencv
#
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

#
# include qt5
#
find_package( Qt5 COMPONENTS Core Gui Widgets Test Concurrent OpenGL REQUIRED )
find_package( OpenMP REQUIRED ) # include openmp (necessary for Qt5)
find_package( GTK REQUIRED )# include GTK (necessary for Qt5)

#
# include local libraries
#
include_directories( ${CMAKE_SOURCE_DIR}/../manager/build )
find_library(
    MY_MANAGER 
    NAMES Manager libManager
    NO_DEFAULT_PATH
)


# add the executable
add_executable(calibration calibration.cpp)

# link libraries
target_link_libraries( calibration ${OpenCV_LIBS} )
target_link_libraries( calibration Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test Qt5::Concurrent Qt5::OpenGL )
target_link_libraries( calibration OpenMP::OpenMP_CXX )
target_link_libraries( calibration ${GTK_USE_FILE} )
target_include_directories( calibration PUBLIC 
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/../manager")
target_link_libraries( calibration ${MY_MANAGER} )