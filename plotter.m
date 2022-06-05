%% Begin
pl = plot_class();
sec_str = "Segundos";
%% Plot
out = sim("proyecto.slx");
frec = [4, 50];
V_dc_follow = out.V_dcs;
V_dc_follow.signals.values = V_dc_follow.signals.values(:,[2,5]);
datas = {
    out.V_dcs, "Tensiónes de los condensadores", ["Voltios", sec_str], [], [];
    out.V_s, "Tensión AB", ["Voltios", sec_str], [], [];
    out.V_acs, "Tensiónes parciales", ["Voltios", sec_str], [], [];
    out.I_ab, "Intensidad AB", ["Voltios", sec_str], [], [];
};
pl.deal_datas(datas, [2, 2], "Proyecto_1_1");
%% End
pause
close all
% clear
