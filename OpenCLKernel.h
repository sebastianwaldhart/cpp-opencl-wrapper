#ifndef _OPEN_CL_KERNEL_
#define _OPEN_CL_KERNEL_

#include <string>
#include <vector>
class OpenCLControl;
#include "OpenCLAbstractObject.h"
#include "OpenCLContext.h"
#include "OpenCLBufferObject.h"
#include "OpenCLEvent.h"

class OpenCLKernel : public OpenCLAbstractObject {
    friend class OpenCLControl;
    private:
        std::vector<OpenCLEvent*> _events;

    protected:
        OpenCLKernel(OpenCLControl* control, std::string name, cl_program* program);
        std::string name;
        cl_kernel kernel;

        virtual void release();

    public:
        void setArgument(unsigned pos, OpenCLBufferObject* bufferObject);
        void setArgument(unsigned pos, size_t size, void* data);

        void enqueueTask();
        OpenCLEvent* enqueueNDRangeKernel(unsigned int dimensions, const size_t* globalWorkSizes, 
                                            const size_t* offset = NULL, const size_t* localWorkSizes = NULL, bool createEvent = false);
        OpenCLEvent* enqueueNDRangeKernel(unsigned int dimensions, const size_t* globalWorkSizes, bool createEvent);
};

#endif // _OPEN_CL_KERNEL