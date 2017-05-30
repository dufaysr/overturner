close all
clearvars

fontsize = 12;
set(groot, 'defaultTextFontSize', fontsize);
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',fontsize);

root = '/home/renaud/Documents/EPL/tfe/overturner/';
textwidth = 16.06;
% Name of the model
model = 'timmermans/0406';
% name of the files
name_info = 'info';
% Import model parameters in the workspace
fprintf('Importing model parameters...\n');
importfileInfo(model,name_info);
ytick = [0 y0 L1-L0];
ztick = [0 z0 H1-H0];
yticklab = {'0','$y_0$','L'};
zticklab = {'0','$z_0$','H'};
% number of times at which stability is computed
% nTimes = 11; ind_iter = 0:nTimes;
times = 0:10:100; times(1) = 1;
ind_iter = 0:10; % hardcoded. Correspond to times 1, 10 and 20 years
iter = 0;
for i = ind_iter
    iter = iter + 1;
    fprintf('\n-----------------ITERATION %d/%d-----------------\n',iter,length(ind_iter));
    nameM = ['M' num2str(i)];
    % Load transition probability matrix from binary output file
    fprintf('Loading Transition probabilities matrix...\n');
    M = loadMbin(model,nameM);
    % Compute clustering using stability criterion at different Markov times
    fprintf('Computing clustering...\n');
    Markov_T = 10.^(-1:0.02:2); % exponential sampling of Markov time is advised in the documentation
    [S, N, VI, C] = stability(M,Markov_T,'directed','plot','teleport',0.01,'L',200);
    set(gcf,'units','centimeters','position',[10 10 textwidth textwidth]);
    set(gcf,'PaperPositionMode','auto')
    set(gcf,'Resize','off')
    print(gcf,['/home/renaud/Documents/EPL/tfe/overturner/matlab/cluster_fig/stab' num2str(i) '.eps'],'-depsc2','-loose');
    fprintf(['Enter the markov times that you want to examine.\n'...
        'Markov time are sampled from %f to %f with an exp time step.\n'],...
        Markov_T(1), Markov_T(end));
    mt = input('Markov times : ');
    close % close stability plot figure
    for imt = 1:length(mt)
        fprintf('\t EXAMINING MARKOV TIME %f\n', mt(imt));
        ind_inf = find(Markov_T <= mt(imt)); ind_inf = ind_inf(end);
        ind_sup = find(Markov_T >= mt(imt)); ind_sup = ind_sup(1);
        ind = ind_inf;
        if ((ind_sup > ind_inf) && (abs(Markov_T(ind_sup)-mt(imt)) < abs(Markov_T(ind_inf)-mt(imt))))
            ind = ind_sup;
        end
        fprintf('\t\tFound %d communities with stability = %f and VI = %f.\n',N(ind),S(ind),VI(ind));
        % Node k corresponds to spatial position iy = k/dimz, iz = mod(k,dimz)
        Cloc = reshape(C(:,ind),nboxz,nboxy);
        disp(Cloc);
        L = L1-L0;
        H = H1-H0;
        dy = L/nboxy; dz = H/nboxz;
        y = linspace(dy/2,L-dy/2,nboxy);
        z = linspace(dz/2,H-dz/2,nboxz);
        dygrid = L/nboxy;
        dzgrid = H/nboxz;
        ygrid = ones(100,1)*([0:dygrid:y0-dygrid y0+dygrid:dygrid:L]); zygrid = linspace(0,H,100)'*ones(1,size(ygrid,2));
        zgrid = ones(100,1)*([0:dzgrid:z0-dzgrid z0+dzgrid:dzgrid:H]); yzgrid = linspace(0,L,100)'*ones(1,size(zgrid,2));
        
        fig = figure('units','centimeters','position',[10 10 .5*textwidth .5*textwidth]);

        set(gcf,'PaperPositionMode','auto')
        set(gcf,'Resize','off')
        %     set(gcf,'Visible', 'off'); % Prevent figure from popping up
        set(gca,'xtick',ytick,'XTickLabel',yticklab)
        set(gca,'ytick',ztick,'YTickLabel',zticklab)
        hold on
        grid on
        imagesc(y,z,Cloc);
        set(gca, 'YDir', 'normal');
        % colormap(lines); % other colormap option
        colormap(colorcube);
        plot(ygrid,zygrid,'--k');
        plot(yzgrid,zgrid,'--k');
        plot(linspace(0,L),ones(1,100)*z0,'--k','linewidth',2);
        plot(ones(1,100)*y0,linspace(0,H),'--k','linewidth',2);
        plot(y0,z0,'.k','MarkerSize',30); 
        xlim([0 L1-L0]);
        ylim([0 H1-H0]);
        xlabel('$y$');
        ylabel('$z$');
        print(fig,['/home/renaud/Documents/EPL/tfe/overturner/matlab/cluster_fig/cluster' num2str(i) '_' num2str(N(ind)) '_.eps'],'-depsc2','-loose');
%         input('Press a key to close the figure');
        pause(3);
        close % close community figure
        %% works too
        % CC = map2trueColors(C,N);
        % figure;
        % image(CC);
    end
end