#include "OpenCLAbstractObject.h"
#include "OpenCLControl.h"

OpenCLAbstractObject::OpenCLAbstractObject(OpenCLControl* ctrl) {
    _control = ctrl;
}

OpenCLAbstractObject::~OpenCLAbstractObject() {
    release();
}

OpenCLContext* OpenCLAbstractObject::getContext() {
    return _control->_context;
}

void OpenCLAbstractObject::release() {
    
}