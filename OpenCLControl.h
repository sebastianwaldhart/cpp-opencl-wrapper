#ifndef _OPEN_CL_CONTROL_
#define _OPEN_CL_CONTROL_

#include <vector>
#include <functional>
#include "OpenCLContext.h"
#include "OpenCLProgram.h"
#include "OpenCLBufferObject.h"
#include "OpenCLEvent.h"
#include "OpenCLKernel.h"
#include "OpenCLAbstractObject.h"

class OpenCLControl {
    friend class OpenCLAbstractObject;
    protected:
        OpenCLContext* _context;

    private:
        std::vector<OpenCLAbstractObject*> _objects;
        
    public:
        OpenCLControl(cl_device_type type = CL_DEVICE_TYPE_ALL, unsigned int deviceNum = 0, bool enableProfiling = false);
        OpenCLControl(cl_device_type type, bool enableProfiling, unsigned int deviceNum = 0);
        ~OpenCLControl();

        void sync();

        OpenCLBufferObject* createBufferObject(size_t size, void* data = NULL, cl_mem_flags flags = CL_MEM_READ_WRITE);
        OpenCLProgram* createProgram(const char* filename);
        OpenCLKernel* createKernel(OpenCLProgram* program, std::string name);

        void waitFor(OpenCLEvent& event);
        void waitFor(size_t size, OpenCLEvent* events);
        void waitFor(std::vector<OpenCLEvent> events);

        unsigned long getMeasuredTime(OpenCLEvent& event);
};

#endif // _OPEN_CL_Control_