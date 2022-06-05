/*
 * File : conti.c
 * Abstract:
 *       An example C-file S-function for measuring two signals and doing 
 *       their product
 *
 */

// Tiene que coincidir con el nombre del archivo y el nombre que hay dentro de la S-Function
#define S_FUNCTION_NAME  MPPT
#define S_FUNCTION_LEVEL 2

#define Tsampling 0.2
#define TPWM Tsampling //Tpwm en seg

// Incluir archivos
#include "simstruc.h"
#include "math.h"

/*Variables globales*/

static float Ppv_1 = 0.0, Vpv_1 = 0.0, Ipv_1 = 0.0;
static float Vmppt_1;
float Vmppt;
float inc_Vmppt = 5.0;

/*================*
 * Build checking *
 *================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *   Setup sizes of the various vectors.
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return; /* Parameter mismatch will be reported by Simulink */
    }
    
    // Numero entradas
    if (!ssSetNumInputPorts(S, 3)) return;
    //ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    // Dimension Entrada 0
    ssSetInputPortWidth(S, 0, 1);
    // Hay que ponerlo por cada entrada colocada
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    // Dimension Entrada 1
    ssSetInputPortWidth(S, 1, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    // Dimension Entrada 2
    ssSetInputPortWidth(S, 2, 1);
    ssSetInputPortDirectFeedThrough(S, 2, 1);

    // Numero salidas
    if (!ssSetNumOutputPorts(S,1)) return;
    //ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);
    // Dimensiones de cada salida
    ssSetOutputPortWidth(S, 0, 1);
    
    // Un unico tiempo de muestreo
    ssSetNumSampleTimes(S, 1);

    /* specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    /* Take care when specifying exception free code - see sfuntmpl_doc.c */
    ssSetOptions(S,
                 SS_OPTION_WORKS_WITH_CODE_REUSE |
                 SS_OPTION_EXCEPTION_FREE_CODE |
                 SS_OPTION_USE_TLC_WITH_ACCELERATOR);
}


/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    Specifiy that we inherit our sample time from the driving block.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    //ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetSampleTime(S, 0, Tsampling);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S); 
}

#define MDL_START                      /* Change to #undef to remove function */
#if defined(MDL_START)
/* Function: mdlStart ==========================================================
 * Abstract:
 *
 */
static void mdlStart(SimStruct *S)
{
    Ppv_1 = 6000, Vpv_1 = 320.0, Ipv_1 = 20.0;
    Vmppt_1 = 310.0;
}
#endif /*  MDL_START */


/* Function: mdlOutputs =======================================================
 * Abstract:
 *    
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    float Ipv; //Corriente por la bobina
    float Vpv;//Tension entrada
    float Ppv;
    float inc_Ppv;
    float inc_Vpv;
    float inc_Ipv;
    float aux_Vmppt;
    int selector;
    float t = ssGetT(S); /* Actualización del tiempo actual */

    //int_T             i;
    InputRealPtrsType pVpv = ssGetInputPortRealSignalPtrs(S,0); // Puntero a la tension (direccion de memoria entrada 0)
    InputRealPtrsType pIpv = ssGetInputPortRealSignalPtrs(S,1); // Puntero a la intensidad (direccion de memoria entrada 1)
    InputRealPtrsType pSelector = ssGetInputPortRealSignalPtrs(S,2); // Puntero a la intensidad (direccion de memoria entrada 2)
    real_T            *y1    = ssGetOutputPortRealSignal(S,0); // Datos de salida en la direccion de memoria de salida 0
    real_T            *y2    = ssGetOutputPortRealSignal(S,1);
    
    //Medidas
    // Pasar del puntero a la variable
    Vpv = *pVpv[0];
    Ipv = *pIpv[0];
    selector = (int) *pSelector[0];

    // Calculo incrementos (comun a ambos metodos)
    Ppv = Vpv*Ipv;
    inc_Ppv = Ppv - Ppv_1;
    inc_Vpv = Vpv - Vpv_1;
    inc_Ipv = Ipv - Ipv_1;

    switch (selector){
        // P&O
        case 0:        
            // Incremento de P = 0
            if (inc_Ppv == 0){
                Vmppt = Vmppt_1;
            }
            else{
                // Incremento P > 0
                if (inc_Ppv > 0){
                    // Incremento Vpv > 0 -> Vmppt sube
                    if (inc_Vpv > 0){
                        Vmppt = Vmppt_1 + inc_Vmppt;
                    }
                    // Incremento Vpv < 0 -> Vmppt baja
                    else{
                        Vmppt = Vmppt_1 - inc_Vmppt;
                    }
                }
                // Incremento P < 0
                else{
                    // Incremento Vpv > 0 -> Vmppt baja
                    if (inc_Vpv > 0){
                        Vmppt = Vmppt_1 - inc_Vmppt;
                    }
                    // Incremento Vpv < 0 -> Vmppt sube
                    else{
                        Vmppt = Vmppt_1 + inc_Vmppt;
                    }
                }
            }
            break;

        // Conductancia incremental
        case 1:
            // Incremento Vpv = 0
            if (inc_Vpv == 0){
                // Incremento Ipv = 0
                if(inc_Ipv == 0){
                    Vmppt = Vmppt_1;
                }
                // Incremento Ipv =/= 0
                else{
                    // Incremento Ipv > 0
                    if (inc_Ipv > 0){
                        Vmppt = Vmppt_1 + inc_Vmppt;
                    }
                    // Incremento Ipv < 0
                    else {
                        Vmppt = Vmppt_1 - inc_Vmppt;
                    }
                }
            }
            // Incremento Vpv =/= 0
            else{
                if ((inc_Ipv/inc_Vpv) == -(Ipv/Vpv)){
                    Vmppt = Vmppt_1;
                }
                else{
                    if ((inc_Ipv/inc_Vpv) > -(Ipv/Vpv)){
                        Vmppt = Vmppt_1 + inc_Vmppt;        
                    }
                    else {
                        Vmppt = Vmppt_1 - inc_Vmppt;
                    }
                }
            }
            break;

        default:
            Vmppt = -1;
    }

    aux_Vmppt = Vmppt_1;

    // Actualizacion parametros
    Ppv_1 = Ppv;
    Vpv_1 = Vpv;
    Ipv_1 = Ipv;
    Vmppt_1 = Vmppt;

    // Escribo el valor en las respectivas direcciones de memoria
    y1[0] = Vmppt;
}


/* Function: mdlTerminate =====================================================
 * Abstract:
 *    No termination needed, b560ut we are required to have this routine.
 */
static void mdlTerminate(SimStruct *S)
{
}



#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
