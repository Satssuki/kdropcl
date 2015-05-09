cmake_minimum_required (VERSION 2.8.x)
project (CLNeuralNetwork)

# Set version number
set (CLNeuralNetwork_VERSION_MAJOR 1)
set (CLNeuralNetwork_VERSION_MINOR 0)
set (CMAKE_CXX_COMPILER g++-4.8)
set (CMAKE_BUILD_TYPE Release)

add_definitions(
  -std=c++11
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/CLNeuralNetworkConfig.h.in"
  "${PROJECT_BINARY_DIR}/CLNeuralNetworkConfig.h"
)
 
# add the binary tree to the search path for include files
# so that we will find CLNeuralNetwork
include_directories("${PROJECT_SOURCE_DIR}/modules")

# add our subdirectories
add_subdirectory("${PROJECT_SOURCE_DIR}/modules") 

add_executable(CLNeuralNetwork quant.cpp)

target_link_libraries(CLNeuralNetwork OpenCL Quant)