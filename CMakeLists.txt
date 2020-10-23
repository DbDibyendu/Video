# Set the CMake minimum version
cmake_minimum_required(VERSION 3.5)

# Declare the Project version and the Language that the project is written in
project(VideoComponent VERSION 1.0.0)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
# This will find the Qt5 files. You will need a QT5_DIR env variable
find_package(Qt5Widgets REQUIRED) # Equivalent of QT += widgets

#  point your compiler to build with C++14
set(CMAKE_CXX_STANDARD 14)
 # This is equal to QMAKE_CXX_FLAGS += -std=c++0x
set(CMAKE_CXX_STANDARD 11)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)
# Instruct CMake to run rcc automatically when needed.
set(CMAKE_AUTORCC ON)


SET(SRC src/VideoFunctions.cpp)
SET(SOURCES examples/example1.cpp src/dispay.cpp src/VideoFunctions.cpp) 
SET(HEADERS include/dispay.h )
SET(UI src/dispay.ui)


# Global Install Directories

include(GNUInstallDirs)

# include the directories
include_directories(include)


# Find and add the OpenCV dependencies
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add rules to create the binary files
add_executable(example1  ${SOURCES} ${HEADERS} ${UI} ${SRC} )
# add_executable(example2 examples/example2.cpp ${SRC})


# Link all the Header

target_link_libraries( example1 Qt5::Widgets ${OpenCV_LIBS})
# target_link_libraries( example2 PUBLIC  ${OpenCV_LIBS})





