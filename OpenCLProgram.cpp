#include "OpenCLProgram.h"
#include <iostream>
#include <fstream>

OpenCLProgram::OpenCLProgram(OpenCLControl* ctrl, const char* filename)
    : OpenCLAbstractObject::OpenCLAbstractObject(ctrl) {
    std::ifstream file;
    size_t length;
    file.open(filename);
    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);
    char* source = new char[length];
    file.read(source, length);
    file.close();

    program = getContext()->createProgram(source, length);
    getContext()->buildProgram(program);
    delete(source);
}

void OpenCLProgram::release() {
    getContext()->releaseProgram(program);
}

cl_program OpenCLProgram::getProgram() {
    return program;
}