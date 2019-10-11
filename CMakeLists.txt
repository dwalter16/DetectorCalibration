#####################################################################
#   Tools for calibrating detectors in the IRIS setup at TRIUMF.
#
#####################################################################

cmake_minimum_required(VERSION 3.8)
project(DetectorCalibration VERSION 0.9 LANGUAGES CXX)

# Tell cmake where to look for special cmake modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#Specify default build type (This is important for optimisation)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Find files to compile
file(GLOB_RECURSE HEADERS "include/*")
file(GLOB_RECURSE SOURCE  "source/*.cpp")
add_library(DetectorCalibration SHARED ${HEADERS} ${SOURCE})
add_executable(calibrate main.cpp)

target_include_directories(DetectorCalibration
  PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_include_directories(calibrate
  PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

#We use modern c++ features.
target_compile_features(DetectorCalibration PUBLIC cxx_std_11)
target_compile_features(calibrate PRIVATE cxx_std_11)

#New Find-module providing the imported target ROOT::Libraries.
#  ROOT::Libraries has the most common targets:
#    Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics 
#    MathCore Thread MultiProc
find_package (ROOT REQUIRED)

target_link_libraries(DetectorCalibration
  PUBLIC
    ROOT::Libraries   
)

target_link_libraries(calibrate
  PRIVATE
    ROOT::Libraries
    DetectorCalibration
)

#Enable doxygen build by default.
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
  # set input and output files
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # request to configure the file
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  # note the option ALL which allows to build the docs together with the application
  add_custom_target(documentation ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
