#include "OpenCLBufferObject.h"

OpenCLBufferObject::OpenCLBufferObject(OpenCLControl* ctrl, cl_mem_flags flags, size_t data_size, void* data_ptr)    
    : OpenCLAbstractObject::OpenCLAbstractObject(ctrl) {
    size = data_size;
    data = data_ptr;

    memObject = getContext()->createBuffer(flags, size, data);
}

void OpenCLBufferObject::release() {
    getContext()->releaseBuffer(memObject);
}

void OpenCLBufferObject::enqueueReadBuffer(size_t size, void* data, int blocking) {
    getContext()->enqueueReadBuffer(memObject, size, data, blocking ? CL_TRUE : CL_FALSE);
}

void OpenCLBufferObject::setAsArgument(unsigned int pos, cl_kernel kernel) {
    getContext()->setKernelArg(kernel, pos, sizeof(cl_mem), (void*) &memObject);
}

void OpenCLBufferObject::enqueueWriteBuffer(size_t size, const void* data, size_t offset, int blocking) {
    getContext()->enqueueWriteBuffer(memObject, size, data, offset, blocking ? CL_TRUE : CL_FALSE);
}