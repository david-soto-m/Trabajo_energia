classdef plot_class
    % Auto plotting class requires data structrue as 
    %{struct_with_time, "plot_title", [Ylabel, Xlabel], [[start]?, periods, frec], ["Legends"];}
    properties
        box
        grid
        line_width
        leg_end_loc
        leg_fontsize
        font_size
        tick_size
        out_time_except
        figdir
        dir_div 
        ax_fontsize
    end
    methods
        function self = plot_class()
            self.box = 'off';
            self.grid = 'off';
            self.ax_fontsize=20;
            self.font_size = 20;
            self.leg_fontsize=20;
            self.leg_end_loc='best';
            self.line_width = 2;
            self.out_time_except = MException('PlotClass:OutOfTime','Time range out of bounds');
            self.figdir = 'fig';
            self.dir_div = '/';
            self.tick_size = 15;
        end
        function plo = plot_auto(self,ax, data)
        % ax is the ax where the plot should be made.
        % data is of the form of
        %{data, title, {y_title, Xtitle}, {start, per, frec}, {List of Legends}}
            if size(data{1})==[1 1]
                if isempty(data{4})
                    plo=self.plot_whole(ax,data);
                else
                    plo=self.plot_period(ax,data);
                end
            else
                plo=self.plot_mat(ax,data);
            end
            ax.XLabel.String = data{3}(2);
            ax.XLabel.FontSize = self.ax_fontsize;
            ax.YLabel.String = data{3}(1);
            ax.YLabel.FontSize = self.ax_fontsize;
            ax.Box = self.box;
            ax.XGrid = self.grid;
            ax.YGrid = self.grid;
            ax.Title.FontSize=self.font_size;
            ax.Title.String=data{2};
            ax.FontSize=self.ax_fontsize;
            if ~isempty(data{5})
                lgnd = legend(ax);
                lgnd.Location = self.leg_end_loc;
                lgnd.FontSize= self.leg_fontsize;
                lgnd.String = data{5};
            end
        end
        function pl = plot_aux(self,ax,time,data)
        % Plots data over same graph assuming that multiple columns means more
        % graphs to be stuck on top of each other
            for ii = 1 : size(data,2)
                pl = plot(ax,...
                        time,data(:,ii),...
                        "LineWidth",self.line_width);
                ax.XLim=[time(1), time(length(time))];
                hold on;
            end
            hold off;
        end
        function plo = plot_period(self,ax,data)
            [time, data] = self.trimmer(data);
            plo = self.plot_aux(ax, time, data);
        end
        function plo = plot_whole(self,ax,data)
            % 'whole' []
            data = data{1};
            plo = self.plot_aux(ax,data.time, data.signals.values);
        end
        function plo = plot_mat(self,ax,data)
            % 'whole' []
            data = data{1};
            plo = self.plot_aux(ax,data(1,:), data(2, :));
        end
        function [time, data_mod, indexes] = trimmer(~,data)
            sel = data{4};
            data = data{1};
            last = data.time(length(data.time));
            if length(sel) == 2
                min_time= last - sel(1)/sel(2);
                if min_time < data.time(1)|| min_time > last || sel(1)*sel(2) < 0
                    throw(self.out_time_except)
                end
                indexes = (data.time >= min_time) & data.time <= last;
            elseif length(sel) == 3
                time_per = sort([sel(1) + sel(2)/sel(3),sel(1)]);
                if time_per(1)<data.time(1) || time_per(2)>last 
                    throw(self.out_time_except)
                end
                indexes = (data.time<= time_per(2)) & data.time >= time_per(1);
            end
            time=data.time(indexes);
            data_mod=zeros(sum(indexes),size(data.signals.values,2));
            for ii = 1 : size(data.signals.values,2)
                whole_data = data.signals.values(:,ii);
                data_mod(:,ii) = whole_data(indexes);
            end
        end
        function [fig,tiles] = tile_generator(~,rows,cols)
            fig=figure;
            tiles = tiledlayout( ...
                fig, ...
                rows, ...
                cols, ...
                'Padding', 'none', ...
                'TileSpacing', 'compact' ...
              );
        end
        function deal_datas(self,datas,disposition, save_name)
        %datas as needed by plot_auto, disposition as [rows columns], save_name, name of the file to be sav
            [fig, tiles]=self.tile_generator(disposition(1),disposition(2));
            for ii=1:size(datas,1)
                data = datas(ii,:);
                ax = nexttile(tiles);
                plt = self.plot_auto(ax,data);
            end
            if ~exist(self.figdir, 'dir')
                mkdir(self.figdir)
            end
            fig.Color = [1, 1, 1];
            fig.Units = 'normalized';
            fig.Position = [0, 0, 1, 1];
            pause;
            saveas(plt,strcat(self.figdir,self.dir_div,save_name),'epsc');
            saveas(plt,strcat(self.figdir,self.dir_div,save_name),'png');
        end
    end
end
