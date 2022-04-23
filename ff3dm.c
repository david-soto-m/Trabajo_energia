#define S_FUNCTION_NAME ff3dm
#define S_FUNCTION_LEVEL 2

#define TPWM Tsampling //Tpwm en seg

#include "simstruc.h"
#define Tsampling 0.0005

#include "lib/ffm_3d_modulator.h"


static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    if (!ssSetNumInputPorts(S, 4)) return;
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortWidth(S, 1, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortWidth(S, 2, 3);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortWidth(S, 3, 3);
    ssSetInputPortDirectFeedThrough(S, 3, 1);


    if (!ssSetNumOutputPorts(S,1)) return;
    ssSetOutputPortWidth(S, 0, 3);

    ssSetNumSampleTimes(S, 1);

    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S,
                 SS_OPTION_WORKS_WITH_CODE_REUSE |
                 SS_OPTION_EXCEPTION_FREE_CODE |
                 SS_OPTION_USE_TLC_WITH_ACCELERATOR);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, Tsampling);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_START
#if defined(MDL_START)
static void mdlStart(SimStruct *S) {}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{

    // BEGIN Declarations
    InputRealPtrsType pV_grid = ssGetInputPortRealSignalPtrs(S, 0);
    InputRealPtrsType pI_ab = ssGetInputPortRealSignalPtrs(S, 1);
    InputRealPtrsType pV_cond = ssGetInputPortRealSignalPtrs(S, 2);
    InputRealPtrsType pV_cond_ref = ssGetInputPortRealSignalPtrs(S,3);

    real_T *y1 = ssGetOutputPortRealSignal(S, 0);
    // END

    // BEGIN Assignation
    float V_grid, I_ab;
    float V_c1, V_c2, V_c3;
    float V_c1_ref, V_c2_ref, V_c3_ref;
    float P_dc_ref, P_dc;
    V_grid = *pV_grid[0];
    I_ab = *pI_ab[0];
    V_c1 = *pV_cond[0];
    V_c2 = *pV_cond[1];
    V_c3 = *pV_cond[2];
    V_c1_ref = *pV_cond_ref[0];
    V_c2_ref = *pV_cond_ref[1];
    V_c3_ref = *pV_cond_ref[2];

    // BEGIN Modulator
    ffm_3d_args args = {
      .I_ab = I_ab,
      .V_ab_ref = V_grid,
      .V_c = {V_c1, V_c2, V_c3},
      .V_c_ref = {V_c1_ref, V_c2_ref, V_c3_ref},
    };
    ffm_3d_res res = ffm_3d(args);
    // END

    // BEGIN Output
    y1[0] = res.delta[0];
    y1[1] = res.delta[1];
    y1[2] = res.delta[2];
    // END
}

static void mdlTerminate(SimStruct *S){}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
