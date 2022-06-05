%% Begin
pl = plot_class();
sec_str = "Segundos";
%% Plot
% out = sim("proyecto.slx");
datas = {
    out.T_room, "Temperatura", ["ºC", sec_str], [], ["referencia", "real"];
    out.V_room, "Tensión", ["Voltios", sec_str], [], ["referencia", "real"];
    out.P_room, "Potencia", ["Watios", sec_str], [], [];
};
pl.deal_datas(datas, [3, 1], "MPC/Temps");
datas = {
%     out.V_dcs, "Voltajes de las placas", ["Voltios", sec_str], [0.01, Tsim - 0.01, 1], [];
    out.V_dcs, "Voltajes de las placas", ["Voltios", sec_str], [4, 5], [];
    out.V_s, "Voltajes de salida", ["Voltios", sec_str], [4, 50], [];
%     out.V_room, "Tensión", ["Voltios", sec_str], [], ["referencia", "real"];
%     out.P_room, "Potencia", ["Watios", sec_str], [], [];
};
pl.deal_datas(datas, [2, 1], "ff/tension");

datas = {
    out.P_placas, "Potencia de las placas", ["Watios", sec_str], [], [];
    out.P_room, "Potencia de la habitación", ["Watios", sec_str], [], [];
    out.P_vol, "Potencia volcada a red", ["Watios", sec_str],[],[];
};
pl.deal_datas(datas, [3, 1], "res/pots");

%% End
close all
% clear
