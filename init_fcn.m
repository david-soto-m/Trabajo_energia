mex ctrl_V_solar.c lib/ffm_3d_modulator.c;

Tint = 1e-6;
Ts = 0.001666;

L_s = 5e-3;
V_source = 230 * sqrt(2);
C_load = 5e-2;
C_init_v = 300;
T_ext = 25;
Irr = 1000;

shift = (180/3)/360 * Ts;