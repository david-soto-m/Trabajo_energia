%% Begin
pl = plot_class();
sec_str = "Segundos";
%% Plot
% out = sim("proyecto.slx");
% frec = [4, 50];
datas = {
    out.V_dcs, "Tensiónes 1, 2, 3", ["Voltios", sec_str], [1, 8, 1], ["1", "2", "3"];
};
pl.deal_datas(datas, [1, 1], "MPPT/tens");
%% End
pause
close all
% clear
