#ifndef _OPEN_CL_CONTEXT_
#define _OPEN_CL_CONTEXT_

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <iostream>

class OpenCLContext {
    public:
        cl_device_id                    device_id;
        cl_device_id*                   device_ids;
        cl_device_type                  device_type;
        cl_context                      context;
        cl_command_queue                command_queue;
        cl_command_queue_properties     command_queue_props;
        cl_platform_id*                 platform_ids;
        cl_platform_id                  platform_id;
        cl_uint                         ret_num_devices;
        cl_uint                         ret_num_platforms;

        unsigned int deviceNum;

        OpenCLContext(cl_device_type type = CL_DEVICE_TYPE_DEFAULT, unsigned int deviceNum = 0, 
                        cl_command_queue_properties props = 0);

        void getPlatformIds();
        void getDeviceIds();

        void createContext();
        void releaseContext();

        void createCommandQueue();
        void releaseCommandQueue();

        cl_mem createBuffer(cl_mem_flags flags, size_t size, void* data);
        void releaseBuffer(cl_mem memObject);
        void enqueueReadBuffer(cl_mem memObject, size_t size, void* data, cl_bool blocking);
        void enqueueWriteBuffer(cl_mem memObject, size_t size, const void* data, size_t offset, cl_bool blocking);

        cl_program createProgram(char* source, size_t size);
        void buildProgram(cl_program program);
        cl_kernel createKernel(const char* name, cl_program program);
        void setKernelArg(cl_kernel kernel, unsigned int pos, size_t size, void* data);
        void releaseProgram(cl_program program);
        void releaseKernel(cl_kernel kernel);

        void enqueueTask(cl_kernel kernel);
        cl_event enqueueNDRangeKernel(cl_kernel kernel, unsigned int dimensions, const size_t* offset, 
                            const size_t* globalWorkSizes, const size_t* localWorkSizes, bool event);

        void waitForEvents(size_t num, cl_event* events);

        cl_ulong getProfilingInfo(cl_event event, cl_profiling_info info);

        void synchronize();

    private:
        void handleError(cl_int error);
};

#endif // _OPEN_CL_CONTEXT_