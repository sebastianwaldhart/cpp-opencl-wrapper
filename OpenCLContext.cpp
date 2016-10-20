#include "OpenCLContext.h"

OpenCLContext::OpenCLContext(cl_device_type type, unsigned int device, cl_command_queue_properties props) {
    device_id = NULL;
    device_type = type;
    context = NULL;
    command_queue = NULL;
    platform_id = NULL;
    platform_ids = NULL;
    command_queue_props = props;

    deviceNum = device;
}

void OpenCLContext::createCommandQueue() {
    cl_int ret;
    command_queue = clCreateCommandQueue(context, device_id, command_queue_props, &ret);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't create command queue";
    }
}

void OpenCLContext::createContext() {
    getPlatformIds();
    getDeviceIds();

    cl_int ret;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't create context";
    }
}

void OpenCLContext::releaseCommandQueue() {
    cl_int ret = clReleaseCommandQueue(command_queue);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't release command queue";
    }
}

void OpenCLContext::releaseContext() {
    cl_int ret = clReleaseContext(context);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't release context";
    }
}

void OpenCLContext::getPlatformIds() {
    cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't get platform ids";
    }

    platform_ids = (cl_platform_id*) malloc(sizeof(cl_platform_id) * ret_num_platforms);
    ret = clGetPlatformIDs(ret_num_platforms, platform_ids, NULL);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        free(platform_ids);
        throw "can't get platform ids";
    }

    platform_id = platform_ids[0];
    free(platform_ids);
}

void OpenCLContext::getDeviceIds() {
    cl_int ret = clGetDeviceIDs(platform_id, device_type, 0, NULL, &ret_num_devices);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't get device ids";
    }

    if(deviceNum > ret_num_devices) deviceNum = ret_num_devices - 1;
    device_ids = (cl_device_id*) malloc(sizeof(device_id) * ret_num_devices);
    ret = clGetDeviceIDs(platform_id, device_type, ret_num_devices, device_ids, NULL);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't get device ids";
    }

    device_id = device_ids[deviceNum];
    free(device_ids);
}

cl_mem OpenCLContext::createBuffer(cl_mem_flags flags, size_t size, void* data) {
    cl_int ret;
    cl_mem mem = clCreateBuffer(context, flags, size, data, &ret);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't create buffer";
    }

    return mem;
}

void OpenCLContext::releaseBuffer(cl_mem memObject) {
    cl_int ret = clReleaseMemObject(memObject);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't release buffer";
    }
}

void OpenCLContext::enqueueReadBuffer(cl_mem memObject, size_t size, void* data, cl_bool blocking) {
    cl_int ret = clEnqueueReadBuffer(command_queue, memObject, blocking, 0, size, data, 0, NULL, NULL);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't read buffer";
    }
}

void OpenCLContext::enqueueWriteBuffer(cl_mem memObject, size_t size, const void* data, size_t offset, cl_bool blocking) {
    cl_int ret = clEnqueueWriteBuffer(command_queue, memObject, blocking, offset, size, data, 0, NULL, NULL);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't write buffer";
    }
}

cl_program OpenCLContext::createProgram(char* source, size_t size) {
    cl_int ret;
    cl_program program = clCreateProgramWithSource(context, 1, (const char**) &source, (const size_t*) &size, &ret);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't create program";
    }

    return program;
}

void OpenCLContext::buildProgram(cl_program program) {
    cl_int ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        if(ret == CL_BUILD_PROGRAM_FAILURE) {
            size_t logSize;
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
            char *log = (char *) malloc(logSize);
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, logSize, log, NULL);
            std::cerr << log << std::endl;
            free(log);
        }
        throw "can't build program";
    }
}

cl_kernel OpenCLContext::createKernel(const char* name, cl_program program) {
    cl_int ret;
    cl_kernel kernel = clCreateKernel(program, name, &ret);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't create kernel";
    }

    return kernel;
}

void OpenCLContext::setKernelArg(cl_kernel kernel, unsigned int pos, size_t size, void* data) {
    cl_int ret = clSetKernelArg(kernel, pos, size, data);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't set kernel argument";
    }
}

void  OpenCLContext::releaseProgram(cl_program program) {
    cl_int ret = clReleaseProgram(program);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't release program";
    }
}

void  OpenCLContext::releaseKernel(cl_kernel kernel) {
    cl_int ret = clReleaseKernel(kernel);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't release kernel";
    }
}

void OpenCLContext::enqueueTask(cl_kernel kernel) {
    cl_int ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't enqueue task";
    }
}

cl_event OpenCLContext::enqueueNDRangeKernel(
    cl_kernel kernel, unsigned int dimensions, const size_t* offset, const size_t* globalWorkSizes, const size_t* localWorkSizes, bool event) {

    cl_event e;
    cl_int ret = clEnqueueNDRangeKernel(command_queue, kernel, dimensions, offset, globalWorkSizes, localWorkSizes, 0, NULL, event ? &e : NULL);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't enqueue task";
    }

    return e;
}

void OpenCLContext::waitForEvents(size_t num, cl_event* events) {
    cl_int ret = clWaitForEvents(num, events);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't enqueue task";
    }
}

cl_ulong OpenCLContext::getProfilingInfo(cl_event event, cl_profiling_info info) {
    cl_ulong value;
    cl_int ret = clGetEventProfilingInfo(event, info, sizeof(cl_ulong), &value, NULL);

    if(ret != CL_SUCCESS) {
        handleError(ret);
        throw "can't enqueue task";
    }

    return value;
}

void OpenCLContext::synchronize() {
    if(command_queue != NULL) {
        clFlush(command_queue);
        clFinish(command_queue);
    }
}

void OpenCLContext::handleError(cl_int error) {
    switch(error) {
        case CL_INVALID_PLATFORM: 
            std::cerr << "ERROR: platform is not valid (CL_INVALID_PLATFORM)" << std::endl; break;
        case CL_INVALID_DEVICE_TYPE: 
            std::cerr << "ERROR: device_type is not valid (CL_INVALID_DEVICE_TYPE)" << std::endl; break;
        case CL_DEVICE_NOT_FOUND: 
            std::cerr << "ERROR: no device found matching device type (CL_DEVICE_NOT_FOUND)" << std::endl; break;
        case CL_INVALID_VALUE: 
            std::cerr << "ERROR: wrong parameter (CL_INVALID_VALUE)" << std::endl; break;
        case CL_INVALID_DEVICE:
            std::cerr << "ERROR: devices contains an invalid device or a device does not match device_type (CL_INVALID_DEVICE)" << std::endl; break;
        case CL_DEVICE_NOT_AVAILABLE:
            std::cerr << "ERROR: a device is currently not available (CL_DEVICE_NOT_AVAILABLE)" << std::endl; break;
        case CL_OUT_OF_HOST_MEMORY:
            std::cerr << "ERROR: can't require resources on host (CL_OUT_OF_HOST_MEMORY)" << std::endl; break;
        case CL_OUT_OF_RESOURCES:
            std::cerr << "ERROR: can't require resources on device (CL_OUT_OF_RESOURCES)" << std::endl; break;
        case CL_INVALID_QUEUE_PROPERTIES:
            std::cerr << "ERROR: properties for queue are not supported by device (CL_INVALID_QUEUE_PROPERTIES)" << std::endl; break;
        case CL_INVALID_COMMAND_QUEUE:
            std::cerr << "ERROR: invalid command queue (CL_INVALID_COMMAND_QUEUE)" << std::endl; break;
        case CL_INVALID_CONTEXT:
            std::cerr << "ERROR: invalid context (CL_INVALID_CONTEXT)" << std::endl; break;
        case CL_INVALID_BUFFER_SIZE:
            std::cerr << "ERROR: invalid buffer size (CL_INVALID_BUFFER_SIZE)" << std::endl; break;
        case CL_INVALID_HOST_PTR:
            std::cerr << "ERROR: invalid host pointer (CL_INVALID_HOST_PTR)" << std::endl; break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            std::cerr << "ERROR: can't allocate memory for buffer (CL_MEM_OBJECT_ALLOCATION_FAILURE)" << std::endl; break;
        case CL_INVALID_MEM_OBJECT:
            std::cerr << "ERROR: invalid mem_object (CL_INVALID_MEM_OBJECT)" << std::endl; break;
        case CL_INVALID_PROGRAM:
            std::cerr << "ERROR: invalid program (CL_INVALID_PROGRAM)" << std::endl; break;
        case CL_INVALID_BUILD_OPTIONS:
            std::cerr << "ERROR: invalid build options (CL_INVALID_BUILD_OPTIONS)" << std::endl; break;
        case CL_INVALID_OPERATION:
            std::cerr << "ERROR: invalid operation (CL_INVALID_OPERATION)" << std::endl; break;
        case CL_COMPILER_NOT_AVAILABLE:
            std::cerr << "ERROR: compiler is not available (CL_COMPILER_NOT_AVAILABLE)" << std::endl; break;
        case CL_INVALID_KERNEL_NAME:
            std::cerr << "ERROR: invalid kernel name (CL_INVALID_KERNEL_NAME)" << std::endl; break;
        case CL_INVALID_KERNEL_DEFINITION:
            std::cerr << "ERROR: invalid kernel definition (CL_INVALID_KERNEL_DEFINITION)" << std::endl; break;
        case CL_INVALID_PROGRAM_EXECUTABLE:
            std::cerr << "ERROR: invalid program executable (CL_INVALID_PROGRAM_EXECUTABLE)" << std::endl; break;
        case CL_INVALID_KERNEL:
            std::cerr << "ERROR: invalid kernel (CL_INVALID_KERNEL)" << std::endl; break;
        case CL_INVALID_ARG_INDEX:
            std::cerr << "ERROR: invalid argument index (CL_INVALID_ARG_INDEX)" << std::endl; break;
        case CL_INVALID_ARG_VALUE:
            std::cerr << "ERROR: invalid argument value (CL_INVALID_ARG_VALUE)" << std::endl; break;
        case CL_INVALID_ARG_SIZE:
            std::cerr << "ERROR: invalid argument size (CL_INVALID_ARG_SIZE)" << std::endl; break;
        case CL_INVALID_SAMPLER:
            std::cerr << "ERROR: invalid sampler (CL_INVALID_SAMPLER)" << std::endl; break;
        case CL_INVALID_WORK_GROUP_SIZE:
            std::cerr << "ERROR: invalid work group size (CL_INVALID_WORK_GROUP_SIZE)" << std::endl; break;
        case CL_INVALID_EVENT_WAIT_LIST:
            std::cerr << "ERROR: invalid event wait list (CL_INVALID_EVENT_WAIT_LIST)" << std::endl; break;
        case CL_INVALID_WORK_DIMENSION:
            std::cerr << "ERROR: invalid work dimension (CL_INVALID_WORK_DIMENSION)" << std::endl; break;
        case CL_INVALID_GLOBAL_WORK_SIZE:
            std::cerr << "ERROR: invalid global work size (CL_INVALID_GLOBAL_WORK_SIZE)" << std::endl; break;
        case CL_INVALID_GLOBAL_OFFSET:
            std::cerr << "ERROR: invalid global offset (CL_INVALID_GLOBAL_OFFSET)" << std::endl; break;
        case CL_INVALID_WORK_ITEM_SIZE:
            std::cerr << "ERROR: invalid work item size (CL_INVALID_WORK_ITEM_SIZE)" << std::endl; break;
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            std::cerr << "ERROR: misaligned sub buffer offset (CL_MISALIGNED_SUB_BUFFER_OFFSET)" << std::endl; break;
        case CL_INVALID_IMAGE_SIZE:
            std::cerr << "ERROR: invalid image size (CL_INVALID_IMAGE_SIZE)" << std::endl; break;
        case CL_INVALID_BINARY:
            std::cerr << "ERROR: invalid binary (CL_INVALID_BINARY)" << std::endl; break;
        case CL_BUILD_PROGRAM_FAILURE:
            std::cerr << "ERROR: a error occured while building program (CL_BUILD_PROGRAM_FAILURE)" << std::endl; break;
        case CL_INVALID_EVENT:
            std::cerr << "ERROR: invalid event (CL_INVALID_EVENT)" << std::endl; break;
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            std::cerr << "ERROR: profiling info is not available (CL_PROFILING_INFO_NOT_AVAILABLE)" << std::endl; break;
        default: 
            std::cerr << "ERROR: unknown error occured" << std::endl;
    }
}