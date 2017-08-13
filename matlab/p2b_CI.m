close all

fontsize = 14;
set(groot, 'defaultTextFontSize', fontsize);
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',fontsize);

root = '/home/renaud/Documents/EPL/tfe/code/out/';
textwidth = 16.06;

nboxy = 30; nboxz = 10;
model = 'KhKv2box';
% Kh = loadBinMatrix([model '/Kh2box_a5'],nboxy,nboxz)';
% Kv = loadBinMatrix([model '/Kv2box_a5'],nboxy,nboxz)';
Kh = load([root 'KhKv2box/Kh2box_a5.out']);
Kv = load([root 'KhKv2box/Kv2box_a5.out']);

alpha = .75;
H0 = 0; H1 = 5e3;
L0 = -15e6; L1 = 15e6;
z0 = 4e3;
y0 = 13e6;
zstar = H0 + alpha*H1;
ytick = [L0 L0+y0 0 L1-y0 L1];
ztick = [H0 zstar H1];
yticklab = {'$-L$','$y^-_0$','0','$y^+_0$','$L$'};
zticklab = {'0','$z^* = \alpha H$','$H$'};

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
set(gca,'xtick',ytick,'XTickLabel',yticklab)
set(gca,'ytick',ztick,'YTickLabel',zticklab)
hold on
% grid on

[X1,Y1] = hatch([L0 0],[0 H],dygrid/2,10);
plot(X1,Y1,'-k');
[X2,Y2] = hatch([L0 0],[L0+L/4 H],dygrid/2,-10);
plot(X2,Y2,'-k');
plot(L0+dygrid/2,dzgrid/2,'.k','MarkerSize',20);
% plot(ygrid,zygrid,'--k');
% plot(yzgrid,zgrid,'--k');
% plot((L0+y0)*ones(1,100),linspace(H0,H1),'--k','linewidth',1);
% plot((L1-y0)*ones(1,100),linspace(H0,H1),'--k','linewidth',1);
% plot(linspace(L0,L1),z0*ones(1,100),'--k','linewidth',1);
% plot(zeros(1,100),linspace(H0,H1),'--k');
plot(zeros(1,100),linspace(H0,zstar),'k','linewidth',2);
xlim([L0 L1]);
ylim([H0 H1]);
xlabel('$y$');
ylabel('$z$');
h = legend('$C^1(0)$','$C^2(0)$','$C^3(0)$','Location','SouthEast');
set(h,'fontsize',14);