#ifndef FFM_3D_MODULATOR
#define FFM_3D_MODULATOR

#include <math.h>

// More convenient and synthetic way to deal with arguments without pointers
typedef struct Ffm_3d_Args {
    float V_ab_ref;
    float I_ab;
    float V_c[3];
    float V_c_ref[3];
} ffm_3d_args;

// Avoids funky stuff with statics and pointers to return an array
typedef struct Ffm_3d_Res {
    float delta[3];
} ffm_3d_res;

ffm_3d_res ffm_3d(ffm_3d_args args);

#endif // FFM_3D_MODULATOR
