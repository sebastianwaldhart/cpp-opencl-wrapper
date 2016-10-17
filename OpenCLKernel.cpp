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
}

void OpenCLKernel::enqueueTask() {
    getContext()->enqueueTask(kernel);
}

void OpenCLKernel::enqueueNDRangeKernel(
    unsigned int dimensions, const size_t* globalWorkSizes, const size_t* offset, const size_t* localWorkSizes) {
    
    getContext()->enqueueNDRangeKernel(kernel, dimensions, offset, globalWorkSizes, localWorkSizes);
}