close all
clearvars

fontsize = 12;
set(groot, 'defaultTextFontSize', fontsize);
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',fontsize);

root = '/home/renaud/Documents/EPL/tfe/overturner/';
textwidth = 16.06;

% alphavec = [1,.9,.75,.5,.25,0];
% ind_alpha = [1,9,75,5,25,0];

alphavec = [0.5 0];
ind_alpha = [5 0];
iter = 0;
for i = 1:length(alphavec)
    % Name of the model
    namealpha = num2str(ind_alpha(i));
    model = ['data2box/T10/problem2box_a' namealpha];
    % name of the files
    name_info = 'info';
    % Import model parameters in the workspace
    fprintf('Importing model parameters...\n');
    importfileInfo(model,name_info);
    zstar = H0 + alphavec(i)*H1;
    ytick = [L0 L0+y0 0 L1-y0 L1];
    ztick = [H0 z0 H1];
    yticklab = {'-L','$y^-_0$','0','$y^+_0$','L'};
    zticklab = {'0','$z_0$','H'};
    fprintf('\n-----------------ALPHA = %f-----------------\n',alphavec(i));
    % Load transition probability matrix from binary output file
    fprintf('Loading Transition probabilities matrix...\n');
    M = loadMbin(model,'M0');
    % Compute clustering using stability criterion at different Markov times
    fprintf('Computing clustering...\n');
    Markov_T = 10.^(0:0.02:2); % exponential sampling of Markov time is advised in the documentation
    [S, N, VI, C] = stability(M,Markov_T,'directed','plot','teleport',1e-3,'L',200);
    set(gcf,'units','centimeters','position',[10 10 textwidth textwidth]);
    set(gcf,'PaperPositionMode','auto')
    set(gcf,'Resize','off')
    print(gcf,['/home/renaud/Documents/EPL/tfe/overturner/matlab/fig_2box_tau0_T10/stab_a' num2str(ind_alpha(i)) '.eps'],'-depsc2','-loose');
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
        y = linspace(L0+dy/2,L1-dy/2,nboxy);
        z = linspace(dz/2,H-dz/2,nboxz);
        dygrid = L/nboxy;
        dzgrid = H/nboxz;
        ygrid = ones(100,1)*([L0:dygrid:L0+y0-dygrid L0+y0+dygrid:dygrid:-dygrid ...
            dygrid:dygrid:L1-y0-dygrid L1-y0+dygrid:dygrid:L1]); zygrid = linspace(H0,H1,100)'*ones(1,size(ygrid,2));
        zgrid = ones(100,1)*([H0:dzgrid:z0-dzgrid z0+dzgrid:dzgrid:H1]); yzgrid = linspace(L0,L1,100)'*ones(1,size(zgrid,2));
        
        fig = figure('units','centimeters','position',[10 10 1.1*textwidth .5*textwidth]);
        
        set(gcf,'PaperPositionMode','auto')
        set(gcf,'Resize','off')
        %     set(gcf,'Visible', 'off'); % Prevent figure from popping up
        set(gca,'xtick',ytick,'XTickLabel',yticklab)
        set(gca,'ytick',ztick,'YTickLabel',zticklab)
        hold on
        grid on
        imagesc(y,z,Cloc);
        set(gca, 'YDir', 'normal');
%         colormap(lines); % other colormap option
        colormap(colorcube);
        plot(ygrid,zygrid,'--k');
        plot(yzgrid,zgrid,'--k');
        plot((L0+y0)*ones(1,100),linspace(H0,H1),'--k','linewidth',2);
        plot((L1-y0)*ones(1,100),linspace(H0,H1),'--k','linewidth',2);
        plot(linspace(L0,L1),z0*ones(1,100),'--k','linewidth',2);
        plot(zeros(1,100),linspace(H0,H1),'--k');
        plot(zeros(1,100),linspace(H0,zstar),'k','linewidth',2);
        plot(0,zstar,'.k','MarkerSize',30);
        xlim([L0 L1]);
        ylim([H0 H1]);
        xlabel('$y$');
        ylabel('$z$');
        print(fig,['/home/renaud/Documents/EPL/tfe/overturner/matlab/fig_2box_tau0_T10/cluster_a' num2str(ind_alpha(i)) '_' num2str(N(ind)) '_.eps'],'-depsc2','-loose');
        %         input('Press a key to close the figure');
        pause(3);
        close % close community figure
        %% works too
        % CC = map2trueColors(C,N);
        % figure;
        % image(CC);
    end
end