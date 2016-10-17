#ifndef _OPEN_CL_PROGRAM_
#define _OPEN_CL_PROGRAM_

class OpenCLControl;
#include "OpenCLAbstractObject.h"
#include "OpenCLContext.h"
#include <string>

class OpenCLProgram : public OpenCLAbstractObject {
    friend class OpenCLControl;
    private:
        cl_program program;

    protected:
        OpenCLProgram(OpenCLControl* ctrl, const char* filename);
        virtual void release();

        cl_program getProgram();
};

#endif // _OPEN_CL_PROGRAM_