#include "OpenCLControl.h"
#include <iostream>
#include <vector>
#include <algorithm>

OpenCLControl::OpenCLControl(cl_device_type type, unsigned int deviceNum) {
    _context = new OpenCLContext(type, deviceNum);

    _context->createContext();
    _context->createCommandQueue();
}

OpenCLControl::~OpenCLControl() {
    _context->synchronize();

    std::reverse(_objects.begin(), _objects.end());
    for(OpenCLAbstractObject* object : _objects) delete(object);

    _context->releaseCommandQueue();
    _context->releaseContext();

    delete(_context);
}

OpenCLBufferObject* OpenCLControl::createBufferObject(size_t size, void* data, cl_mem_flags flags) {
    OpenCLBufferObject* bufferObject = new OpenCLBufferObject(this, flags, size, data);
    _objects.push_back((OpenCLAbstractObject*) bufferObject);
    return bufferObject;
}

OpenCLProgram* OpenCLControl::createProgram(const char* filename) {
    OpenCLProgram* program = new OpenCLProgram(this, filename);
    _objects.push_back((OpenCLAbstractObject*) program);
    return program;
}

OpenCLKernel* OpenCLControl::createKernel(OpenCLProgram* program, std::string name) {
    cl_program prog = program->getProgram();
    OpenCLKernel* kernel = new OpenCLKernel(this, name, &prog);
    _objects.push_back((OpenCLAbstractObject*) kernel);
    return kernel;
}

void OpenCLControl::sync() {
    _context->synchronize();
}