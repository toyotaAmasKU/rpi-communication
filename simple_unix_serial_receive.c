#define S_FUNCTION_NAME simple_unix_serial_receive
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#include <sys/stat.h>

#define FRDM_BOARD_DEV "/dev/ttyACM0"

int isFRDMBoardExist(void)
{
    struct stat st;
    return !(stat(FRDM_BOARD_DEV, &st));
}

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 2);
    if(ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }
    int_T i;
    int_T parameterWitdth = ssGetNumSFcnParams(S);

    for(i = 0; i < parameterWitdth; i++)
    {
        ssSetSFcnParamTunable(S, i, 0);
    }

    if(!ssSetNumInputPorts(S, 0))
    {
        return;
    }

    if(!ssSetNumOutputPorts(S, 1))
    {
        return;
    }
    ssSetOutputPortDataType(S, 0, SS_UINT8);
    ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);

    ssSetNumSampleTimes(S, 1);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    real_T sampleTime = mxGetPr(ssGetSFcnParam(S, 1))[0];

    ssSetSampleTime(S, 0, sampleTime);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    if(isFRDMBoardExist())
    {
        int8_T *OutputPtrs = (int8_T *)ssGetOutputPortSignal(S, 0);
        int_T elementByte = mxGetPr(ssGetSFcnParam(S, 0))[0];

        FILE *fPtr;
        int8_T buffer[elementByte];
        int8_T i;

        fPtr = fopen(FRDM_BOARD_DEV, "r");
        fgets(buffer, elementByte + 1, fPtr);

        for(i = 0; i < elementByte; i++)
        {
            *OutputPtrs = buffer[i];
            OutputPtrs++;
        }

        fclose(fPtr);
    }
}

static void mdlTerminate(SimStruct *S)
{
}

#ifdef MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif
