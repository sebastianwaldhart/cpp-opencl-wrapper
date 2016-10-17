#ifndef _OPEN_CL_OPEN_CL_BUFFER_OBJECT_
#define _OPEN_CL_OPEN_CL_BUFFER_OBJECT_

class OpenCLControl;
#include "OpenCLAbstractObject.h"
#include "OpenCLContext.h"

class OpenCLBufferObject : public OpenCLAbstractObject {
    friend class OpenCLControl;
    private:
        OpenCLControl* control;
        cl_mem memObject;

    protected:
        OpenCLBufferObject(OpenCLControl* control, cl_mem_flags flags, size_t size, void* data);
        virtual void release();

    public:
        void* data;
        size_t size;

        void enqueueReadBuffer(size_t size, void* data, int blocking = 1);
        void enqueueWriteBuffer(size_t size, const void* data, size_t offset = 0, int blocking = 1);

        void setAsArgument(unsigned int pos, cl_kernel kernel);
};

#endif // _OPEN_CL_OPEN_CL_BUFFER_OBJECT_