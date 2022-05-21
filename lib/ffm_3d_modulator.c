#include "ffm_3d_modulator.h"

// Implicitly private as it's never defined in a public header file
typedef struct Ffm_3d_Control {
    const float K_p;
    const float K_i;
    float epsilon_k_1_l_1;
    float epsilon_k_2_l_1;
} ffm_3d_control;


ffm_3d_control ffm_3d_cont = {
    .K_p = 1.0 / 100.0,
    .K_i = 1,
    .epsilon_k_1_l_1 = 0,
    .epsilon_k_2_l_1 = 0
};

ffm_3d_res ffm_3d(ffm_3d_args args) {
    ffm_3d_res res;

    // BEGIN Equilibrium
    float eq[3];
    float third_V = args.V_ab_ref / 3.0;

    int flag_1 = -1;
    int flag_2 = -1;

    for(int i=0; i<3; i++){
        if (third_V > args.V_c[i]) {
            eq[i] = args.V_c[i];
            flag_1 = i;
            break;
        } else if (third_V < - args.V_c[i]){
            eq[i] = - args.V_c[i];
            flag_2 = i;
            break;
        }
    }

    if (flag_1 != -1 || flag_2 != -1){
        float half_V;
        int flag_3 = -1, flag_4 = -1;
        if (flag_1 != -1) {
            half_V = (args.V_ab_ref - args.V_c[flag_1]) / 2.0;
        } else {
            half_V = (args.V_ab_ref + args.V_c[flag_2]) / 2.0;
        }
        for(int i=0; i < 3; i++){
            if (i != flag_1 + flag_2 + 1){
                if (half_V > args.V_c[i]) {
                    eq[i] = args.V_c[i];
                    flag_3 = i;
                    break;
                } else if (third_V < - args.V_c[i]){
                    eq[i] = - args.V_c[i];
                    flag_4 = i;
                    break;
                }
            }
        }
        if (flag_3 != -1 || flag_4 != -1) {
            for(int i=0; i < 3; i++){
                if (i != flag_1 + flag_2 + 1 && i != flag_3 + flag_4 + 1)
                {
                    if (flag_3 != -1) {
                        eq[i] = (half_V - args.V_c[flag_3]) / 2.0;
                    } else {
                        eq[i] = (half_V + args.V_c[flag_4]) / 2.0;
                    }
                }
            }
        } else {
            for(int i=0; i < 3; i++){
                if (i != flag_1 + flag_2 + 1) {
                    eq[i] = half_V;
                }
            }
        }
    } else {
        for(int i=0; i<3; i++){
            eq[i] = third_V;
        }
    }

    // END Equilibrium

    // BEGIN Correction

    float D_V_c[3];
    float delta[3];
    float epsilon_k_1, chi_k_1, epsilon_k_2, chi_k_2;
    for (int i = 0; i < 3; i++){
        D_V_c[i] = args.V_c_ref[i] - args.V_c[i];
    }
    epsilon_k_1 = (D_V_c[0] - D_V_c[2]) * args.I_ab;
    epsilon_k_2 = (D_V_c[1] - D_V_c[2]) * args.I_ab;
    chi_k_1 = (epsilon_k_1 + ffm_3d_cont.epsilon_k_1_l_1) / 2.0;
    chi_k_2 = (epsilon_k_2 + ffm_3d_cont.epsilon_k_2_l_1) / 2.0;
    delta[0] = eq[0] + ffm_3d_cont.K_p * epsilon_k_1 + ffm_3d_cont.K_i * chi_k_1;
    delta[1] = eq[1] + ffm_3d_cont.K_p * epsilon_k_2 + ffm_3d_cont.K_i * chi_k_2;
    delta[2] = args.V_ab_ref - delta[0] - delta[1];
    ffm_3d_cont.epsilon_k_1_l_1 = epsilon_k_1;
    ffm_3d_cont.epsilon_k_2_l_1 = epsilon_k_2;
    // END Correction

    for(int i=0; i<3; i++){
        res.delta[i] = delta[i] / args.V_c[i];
    }

    return res;
}
