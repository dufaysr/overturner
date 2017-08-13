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

alpha = .6;
H0 = 0; H1 = 5e3;
L0 = -15e6; L1 = 15e6;
z0 = 4e3;
y0 = 13e6;
zstar = H0 + alpha*H1;
ytick = [L0 L0+y0 0 L1-y0 L1];
ztick = [H0 zstar z0 H1];
yticklab = {'$-L$','$y^-_0$','0','$y^+_0$','$L$'};
zticklab = {'0','$z^* = \alpha H$','$z_0$','$H$'};

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
% imagesc(y,z,Kh);
% colorbar
set(gca, 'YDir', 'normal');
% colormap(lines); % other colormap option
% colormap(colorcube);
yp = [L0+y0 L0+y0 L1-y0 L1-y0];
zp = [zstar H1 H1 zstar];
cp = [0.4 0.4 0.4];
fill(yp,zp,cp);
ym1 = [L0 L0 L0+y0 L0+y0];
ym2 = [L1-y0 L1-y0 L1 L1];
zm = [0 H1 H1 0];
cm = [0.7 0.7 0.7];
fill(ym1,zm,cm);
fill([L0+y0 L0+y0 L1-y0 L1-y0],[0 zstar zstar 0],[1 1 1]);
fill(ym2,zm,cm);
plot(ygrid,zygrid,'--k');
plot(yzgrid,zgrid,'--k');
plot((L0+y0)*ones(1,100),linspace(H0,H1),'--k','linewidth',2);
plot((L1-y0)*ones(1,100),linspace(H0,H1),'--k','linewidth',2);
plot(linspace(L0,L1),z0*ones(1,100),'--k','linewidth',2);
plot(zeros(1,100),linspace(H0,H1),'--k');
plot(zeros(1,100),linspace(H0,zstar),'k','linewidth',2);
plot(L0+y0,z0,'.k','MarkerSize',30);
plot(L1-y0,z0,'.k','MarkerSize',30);
xlim([L0 L1]);
ylim([H0 H1]);
xlabel('$y$');
ylabel('$z$');
legend('$K_h = 10^4\;\mathrm{m^2/s}$','$K_h = 10^3\;\mathrm{m^2/s}$',...
    '$K_h = 0\;\mathrm{m^2/s}$','Location','SouthEast');

% fig = figure('units','centimeters','position',[10 10 1.1*textwidth .5*textwidth]);
% set(gcf,'PaperPositionMode','auto')
% set(gcf,'Resize','off')
% %     set(gcf,'Visible', 'off'); % Prevent figure from popping up
% set(gca,'xtick',ytick,'XTickLabel',yticklab)
% set(gca,'ytick',ztick,'YTickLabel',zticklab)
% hold on
% grid on
% imagesc(y,z,Kv);
% colorbar
% set(gca, 'YDir', 'normal');
% colormap(lines); % other colormap option
% % colormap(colorcube);
% plot(ygrid,zygrid,'--k');
% plot(yzgrid,zgrid,'--k');
% plot((L0+y0)*ones(1,100),linspace(H0,H1),'--k','linewidth',2);
% plot((L1-y0)*ones(1,100),linspace(H0,H1),'--k','linewidth',2);
% plot(linspace(L0,L1),z0*ones(1,100),'--k','linewidth',2);
% plot(zeros(1,100),linspace(H0,H1),'--k');
% plot(zeros(1,100),linspace(H0,zstar),'k','linewidth',2);
% plot(0,zstar,'.k','MarkerSize',30);
% xlim([L0 L1]);
% ylim([H0 H1]);
% xlabel('$y$');
% ylabel('$z$');