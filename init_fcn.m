%% Compile
mex ff3dm.c lib/ffm_3d_modulator.c;
mex MPPT.c;

%% Time related
Tsim = 40;
Tint = 5e-5; % s
Ts = 0.0005; % s
% 2 khz ↑
T_pred = Ts*20; % Tendría que escoger esto, yo vería 0.02 o como mucho 0.001
T_Enable = 0.35;

%% Exterior
T_ext = 15; % deg
Irr = 1000; % W / m^2

%%  Electrical
V_source = 230 * sqrt(2); % Volt
C_load = 5e-2; % 50mF
L_s = 5e-3; % 5mH
C_init_v = 315; %Volt
shift = (180/3)/360 * Ts; %deg

%% Room
V_H4 = 300; %V
R_cuarto = 100; %ohm
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

tf_cont=tf(1/C_V,[1 K_cond/C_V]); %% En función de transferencia contínua
ss2c= ss(tf_cont); % En ecuaciónes de estado continua
ss2 = c2d(ss2c, T_pred, 'tustin'); % En ecuaciónes de estado discretas

%% MPC Control
% Tm = Ts*20; % Tiempo de muestreo del control
nx = size(ss2.A,2); % Numero de estados
nu = size(ss2.B,2); % Numero de acciones de control
lambda = 0.4;
U0 = 0;
X0 = 0;
Kobs = 1;
N2 = 5; % Horizonte de prQediccion
Nu = 5; % Horizonte de control

model = ss2;

Temp_ref = [20*ones(Tsim/(T_pred*2),1); 25*ones(Tsim/(T_pred*2),1)];

M = [model.A model.B; zeros(nu,nx) eye(nu)];
N = [model.B; eye(nu)];
Q = [model.C zeros(1,nu)]; %     zeros(ny,nu)

% y_pred = F* x_pred + H *U
% Calculamos F = [Q*M; Q*M^2; ....; Q*M^(N2)
F = zeros(N2,(nx+nu)); %N2 filas, Nx+Nu columnas(dimension de M)
for i=1:N2
    F(i,:) = Q*(M^(i));
end

% Calculamos H ; Matriz triangular con Hij = Q*M^(i-j)*N, dimensiones N2 x Nu
H = zeros(N2,Nu);
for i = 1:N2
    for j = 1:Nu
        if i>=j
            H(i,j) = Q*M^(i-j)*N;
        else
            H(i,j) = 0;
        end
    end
end

% Accion de control y salida
K1 = inv(H'*H + lambda*eye(Nu))*H';
K = K1(1,:); % Solo interesa la primera fila