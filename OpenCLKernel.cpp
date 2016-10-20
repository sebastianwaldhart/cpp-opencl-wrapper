#include "OpenCLKernel.h"

OpenCLKernel::OpenCLKernel(OpenCLControl* ctrl, std::string nm, cl_program* program) 
    : OpenCLAbstractObject::OpenCLAbstractObject(ctrl) {
    name = nm;

    kernel = getContext()->createKernel(name.c_str(), *program);
}

void OpenCLKernel::setArgument(unsigned pos, OpenCLBufferObject* bufferObject) {
    bufferObject->setAsArgument(pos, kernel);
}

void OpenCLKernel::setArgument(unsigned pos, size_t size, void* data) {
    getContext()->setKernelArg(kernel, pos, size, data);
}

void OpenCLKernel::release() {
    getContext()->releaseKernel(kernel);

    for(OpenCLEvent* e : _events) delete(e);
}

void OpenCLKernel::enqueueTask() {
    getContext()->enqueueTask(kernel);
}

OpenCLEvent* OpenCLKernel::enqueueNDRangeKernel(unsigned int dimensions, const size_t* globalWorkSizes, const size_t* offset, 
        const size_t* localWorkSizes, bool createEvent) {

    cl_event event = getContext()->enqueueNDRangeKernel(kernel, dimensions, offset, globalWorkSizes, localWorkSizes, createEvent);

    OpenCLEvent* e  = NULL;
    if(createEvent) {
        e = new OpenCLEvent(event);
        _events.push_back(e);
    }

    return e;
}

OpenCLEvent* OpenCLKernel::enqueueNDRangeKernel(unsigned int dimensions, const size_t* globalWorkSizes, bool createEvent) {
    return enqueueNDRangeKernel(dimensions, globalWorkSizes, NULL, NULL, createEvent);
}