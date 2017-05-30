%% Generate a tikz scheme for the decomposition of the domain into boxes,
%% as explained in section "Description of the method".
% 
close all
fontsize = 14;
set(groot, 'defaultTextFontSize', fontsize); 
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',fontsize);

% Plot the scheme
L = 15;
H = 5;
y0 = 13;
z0 = 4;
nboxy = 15;
nboxz = 10;
textwidth = 16.06;
y = linspace(0,L,nboxy+1);
z = linspace(0,H,nboxz+1);
ygrid = ones(100,1)*y; zygrid = linspace(0,H,100)'*ones(1,size(ygrid,2));
zgrid = ones(100,1)*z; yzgrid = linspace(0,L,100)'*ones(1,size(zgrid,2));

ytick = [0 y0 L];
ztick = [0 z0 H];
yticklab = {'0','$y_0$','L'};
zticklab = {'0','$z_0$','H'};

fig = figure('units','centimeters','position',[10 10 textwidth .8*textwidth]);
set(gcf,'PaperPositionMode','auto')
set(gcf,'Resize','off')
set(gca,'xtick',ytick,'XTickLabel',yticklab)
set(gca,'ytick',ztick,'YTickLabel',zticklab)
hold on
grid on
plot(ygrid,zygrid,'--k');
plot(yzgrid,zgrid,'--k');
plot(linspace(0,L),ones(1,100)*z0,'-k','Linewidth',2);
plot(ones(1,100)*y0,linspace(0,H),'-k','Linewidth',2);
plot(y0,z0,'.k','MarkerSize',25);
xlabel('$y$');
ylabel('$z$');

% Generate tikz file using matlab2tikz
% matlab2tikz('imagesAsPng',true);
matlab2tikz('/home/renaud/Documents/EPL/tfe/latex/fig/clusters/box_scheme.tex','width','.5\textwidth');