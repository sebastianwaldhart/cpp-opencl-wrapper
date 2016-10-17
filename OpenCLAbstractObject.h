#ifndef _OPEN_CL_ABSTRACT_OBJECT_
#define _OPEN_CL_ABSTRACT_OBJECT_

class OpenCLControl;
#include "OpenCLContext.h"

class OpenCLAbstractObject {
    friend class OpenCLControl;
    private:
        OpenCLControl* _control;
    
    protected:
        OpenCLAbstractObject(OpenCLControl* control);
        virtual ~OpenCLAbstractObject();

        virtual void release();

        OpenCLContext* getContext();
};

#endif // _OPEN_CL_ABSTRACT_OBJECT_