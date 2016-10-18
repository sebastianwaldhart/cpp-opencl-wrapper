# cpp-opencl-wrapper

CMakeLists.txt for OpenCL:
'''cmake
cmake_minimum_required (VERSION 2.8)
project(opencl_project)

find_package(OpenCL REQUIRED)

message(STATUS "OpenCL found: ${OPENCL_FOUND}")
message(STATUS "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")

include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

set(WRAPPER <wrapper-folder>/OpenCLControl.cpp opencl/OpenCLControl.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLContext.cpp opencl/OpenCLContext.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLBufferObject.cpp opencl/OpenCLBufferObject.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLProgram.cpp opencl/OpenCLProgram.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLKernel.cpp opencl/OpenCLKernel.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLAbstractObject.cpp opencl/OpenCLAbstractObject.h)

add_executable(main ${WRAPPER} main.cpp)

target_include_directories (main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (main ${OpenCL_LIBRARY})
'''
