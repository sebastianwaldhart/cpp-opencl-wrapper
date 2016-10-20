#ifndef _OPEN_CL_EVENT_
#define _OPEN_CL_EVENT_

class OpenCLControl;
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <iostream>

class OpenCLEvent {
    friend class OpenCLControl;
    protected:
        cl_event event;

    public:
        OpenCLEvent(cl_event e);
        
};

#endif // _OPEN_CL_EVENT_