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
project(Basic)

#
# include opencv
#
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# add the executable
add_executable(basic basic.cpp)

# link libraries
target_link_libraries( basic ${OpenCV_LIBS} )