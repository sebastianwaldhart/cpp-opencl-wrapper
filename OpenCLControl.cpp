#include "OpenCLControl.h"
#include <iostream>
#include <vector>
#include <algorithm>

OpenCLControl::OpenCLControl(cl_device_type type, unsigned int deviceNum, bool enableProfiling) {
    _context = new OpenCLContext(type, deviceNum, enableProfiling ? CL_QUEUE_PROFILING_ENABLE : 0);

    _context->createContext();
    _context->createCommandQueue();
}

OpenCLControl::OpenCLControl(cl_device_type type, bool enableProfiling, unsigned int deviceNum) 
    : OpenCLControl::OpenCLControl(type, deviceNum, enableProfiling) { }

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

void OpenCLControl::waitFor(OpenCLEvent& event) {
    waitFor(1, &event);
}

void OpenCLControl::waitFor(size_t size, OpenCLEvent* events) {
    cl_event e[size];
    for(unsigned int i = 0; i < size; ++i) e[i] = events[i].event;
    _context->waitForEvents(size, e);
}

void OpenCLControl::waitFor(std::vector<OpenCLEvent> events) {
    waitFor(events.size(), &events[0]);
}

unsigned long OpenCLControl::getMeasuredTime(OpenCLEvent& event) {
    cl_ulong start = _context->getProfilingInfo(event.event, CL_PROFILING_COMMAND_START);
    cl_ulong end = _context->getProfilingInfo(event.event, CL_PROFILING_COMMAND_END);

    return end - start;
}