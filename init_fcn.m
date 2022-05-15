%% Compile
mex ff3dm.c lib/ffm_3d_modulator.c;
load data/models.mat;

%% Time related
Tint = 5e-5; % s
Ts = 0.0005; % s
% 2 khz ↑

%% Exterior
T_ext = 15; % deg
Irr = 1000; % W / m^2

%%  Electrical
V_source = 230 * sqrt(2); % Volt
C_load = 5e-2; % 50mF
L_s = 5e-3; % 5mH
C_init_v = 317; %Volt
shift = (180/3)/360 * Ts; %deg

%% fourth
V_H4 = 300;

%% Room
volume_room = 2.7*2.6*2.6; % m^3
ro_air = 1.204; %kg/m^2
c_v = 1004 / 100; % J/(kg K) 
% integral(P, dt) = c_v * m * (T_f - T_o) = C_V * (T_f -T_o)
C_V = c_v * ro_air * volume_room; % J/K

Window_area = 1.1 * 0.9; % m^2
k_glass = 0.96; % (W/m^2)/(m K) 
esp = 6e-3; % 6mm
% P_Cond = k_glass/L * (T_out - T_in) * A_cond = K_cond * (T_out - T_in)
K_cond = Window_area * k_glass / esp; % W/K