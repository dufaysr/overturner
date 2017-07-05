textwidth = 16.06;
close all;


fontsize = 14;
set(groot, 'defaultTextFontSize', fontsize);
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',fontsize);

H = 5e3;
L = 15e6;
alpha = 0.75;
zstar = alpha*H;

% y0 = 11/12;
% z0 = 3.5/4;
y0 = 13e6;
z0 = 4e3;

% Y = load('/home/renaud/Documents/EPL/tfe/overturner/out/data2box/Y.out');
% Z = load('/home/renaud/Documents/EPL/tfe/overturner/out/data2box/Z.out');
% t = load('/home/renaud/Documents/EPL/tfe/overturner/out/data2box/time.out');
Z = Z(:,10);
Y = Y(:,10);
ind1 = Z >=0;
indy = Y > 0;
indz = Z < zstar;
ind2 = indy & indz;
ind3 = ~ind2 & ind1;


y = linspace(-L,L);
z = linspace(0,H);
ytick = [-L -L+y0 0 L-y0 L];
ztick = [0 z0 H];
yticklab = {'$-L$','$y^-_0$','0','$y^+_0$','$L$'};
zticklab = {'0','$z_0$','$H$'};

year = 365*24*3600;
indT = ind3 & (t <= 200*year);
indEnd = length(Z(indT));
for ind = 1
    figure('units','centimeters','position',[10 10 1.1*textwidth .5*textwidth]);
    set(gcf,'PaperPositionMode','auto')
    set(gcf,'Resize','off')
    set(gca,'xtick',ytick,'XTickLabel',yticklab)
    set(gca,'ytick',ztick,'YTickLabel',zticklab)
    hold on
    plot(y,z0*ones(size(y)),':k');
    plot((y0-L)*ones(size(z)),z,':k');
    plot((L-y0)*ones(size(z)),z,':k');
    plot(zeros(100,1),linspace(0,zstar),'k','LineWidth',2);
    plot(Y(indT,ind),Z(indT,ind));
    plot(Y(1,ind),Z(1,ind),'xk','MarkerSize',15,'LineWidth',2);
    plot(Y(indEnd,ind),Z(indEnd,ind),'.k','MarkerSize',15);
    xlabel('$y$');
    ylabel('$z$');
end