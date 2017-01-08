#define S_FUNCTION_NAME simple_unix_serial_send
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

#include <sys/stat.h>

#define FRDM_BOARD_DEV "/dev/ttyACM0"

int is_frdm_board_exist(void)
{
    struct stat st;
    return !(stat(FRDM_BOARD_DEV, &st));
}

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);
    if(ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    if(!ssSetNumInputPorts(S, 1))
    {
        return;
    }
    ssSetInputPortDataType(S, 0, SS_UINT8);
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if(!ssSetNumOutputPorts(S, 0))
    {
        return;
    }

    ssSetNumSampleTimes(S, 1);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    if(is_frdm_board_exist())
    {
        InputPtrsType rawInputPtrs = ssGetInputPortSignalPtrs(S, 0);
        InputUInt8PtrsType inputPtrs = (InputUInt8PtrsType)rawInputPtrs;
        int_T i;
        int_T inputPortWidth = ssGetInputPortWidth(S, 0);

        for(i = 0; i < inputPortWidth; i++)
        {
            FILE* fPtr;
            int8_T data = *inputPtrs[i];

            fPtr = fopen(FRDM_BOARD_DEV, "w");
            fputc(data, fPtr);
            fclose(fPtr);
        }
    }
}

static void mdlTerminate(SimStruct *s)
{
}

#ifdef MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif
