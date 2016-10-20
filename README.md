# C++ OpenCL Wrapper

## How to build?

CMakeLists.txt for OpenCL:
```cmake
cmake_minimum_required (VERSION 2.8)
project(opencl_project)

find_package(OpenCL REQUIRED)

message(STATUS "OpenCL found: ${OPENCL_FOUND}")
message(STATUS "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")

include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

set(WRAPPER <wrapper-folder>/OpenCLControl.cpp <wrapper-folder>/OpenCLControl.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLContext.cpp <wrapper-folder>/OpenCLContext.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLBufferObject.cpp <wrapper-folder>/OpenCLBufferObject.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLProgram.cpp <wrapper-folder>/OpenCLProgram.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLKernel.cpp <wrapper-folder>/OpenCLKernel.h)
set(WRAPPER ${WRAPPER} <wrapper-folder>/OpenCLAbstractObject.cpp <wrapper-folder>/OpenCLAbstractObject.h)

add_executable(main ${WRAPPER} main.cpp)

target_include_directories (main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (main ${OpenCL_LIBRARY})
```

## How to use?

```c++
#include "<wrapper-folder>/OpenCLControl.h"

int main(int argc, char** argv) {
  OpenCLControl* control = new OpenCLControl(CL_DEVICE_TYPE_GPU, 1); // 
default = CL_DEVICE_TYPE_ALL and 0
  
  OpenCLBufferObject* buffer = control->createBufferObject(sizeof(cl_float) * 100);
  
  OpenCLProgram* program = control->createProgram("kernel.cl");
  OpenCLKernel* kernel = control->createKernel(program, "kernelName");
  
  cl_uint X = 10;
  kernel->setArgument(0, sizeof(cl_uint), &X);
  kernel->setArgument(1, buffer);
  
  size_t worksize[2] = [10, 10];
  kernel->enqueueNDRangeKernel(2, (const size_t*) worksize);
  
  kernel->sync();
  
  delete(control); // releases all OpenCL Objects created with control
  
  return 0;
}

```
