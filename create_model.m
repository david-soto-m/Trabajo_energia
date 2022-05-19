out = sim("proyecto.slx");
input = out.V_room1.Data - 300;
output = out.T_room1.Data - 20.65;
input = input( 10/Tint: length(input),:);
output = output( 10/Tint: length(output),:);
systemIdentification;
% pause;
% save data/models tf1 ss1;