function displaySpeed
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
textwidth = 16.06;
close all;

H = 1;
L = 1;
PSI = 1;

y0 = 11/12;
z0 = 3.5/4;

ytick = [0 y0 1];
ztick = [0 z0 1];
yticklab = {'0','$$\frac{y_0}{L}$$','1'};
zticklab = {'0','$$\frac{z_0}{H}$$','1'};

y = linspace(0,1,120);
z = linspace(0,1,100);
[Y,Z] = meshgrid(y,z);
V = v(Y,Z,y0,z0,PSI,L,H);
W = w(Y,Z,y0,z0,PSI,L,H);

figure('units','centimeters','position',[10 10 .5*textwidth .5*textwidth]);
set(gcf,'PaperPositionMode','auto')
set(gcf,'Resize','off')
set(gca,'xtick',ytick,'XTickLabel',yticklab)
set(gca,'ytick',ztick,'YTickLabel',zticklab)
hold on
plot(y,z0*ones(size(y)),':k');
plot(y0*ones(size(z)),z,':k');
% contour(Y,Z,V)
contour(Y,Z,V,-2:-1,'ShowText','on','LabelSpacing',150);
contour(Y,Z,V,[0 0],'ShowText','on','LabelSpacing',150);
contour(Y,Z,V, [2 2],'ShowText','on','LabelSpacing',90);
contour(Y,Z,V, [4 4],'ShowText','on','LabelSpacing',70);
contour(Y,Z,V, [6 6]);
contour(Y,Z,V, [8 8],'ShowText','on','LabelSpacing',1000);
contour(Y,Z,V, [10 12]);
contour(Y,Z,V, [14 14],'ShowText','on','LabelSpacing',1000);
caxis([-20 20])
% colorbar
xlabel('$$y/L$$');
ylabel('$$z/H$$');

figure('units','centimeters','position',[10 10 .5*textwidth .5*textwidth]);
set(gcf,'PaperPositionMode','auto')
% set(gcf,'Resize','off')
set(gca,'xtick',ytick,'XTickLabel',yticklab)
set(gca,'ytick',ztick,'YTickLabel',zticklab)
hold on
plot(y,z0*ones(size(y)),':k');
plot(y0*ones(size(z)),z,':k');
% contour(Y,Z,W,-20:4,'ShowText','on')
contour(Y,Z,W,-18:4:-2);
contour(Y,Z,W,[-20 -20],'ShowText','on','LabelSpacing',1000);
% contour(Y,Z,W,[-15 -15]);
contour(Y,Z,W,[-10 -10],'ShowText','on','LabelSpacing',85.5);
contour(Y,Z,W,[-4 -4],'ShowText','on','LabelSpacing',130);
% contour(Y,Z,W,[0 0],'ShowText','on','LabelSpacing',180);
% contour(Y,Z,W, [2 2],'ShowText','on','LabelSpacing',90);
% contour(Y,Z,W, [4 4],'ShowText','on','LabelSpacing',60);
% contour(Y,Z,W, [6 6],'ShowText','on','LabelSpacing',55);
contour(Y,Z,W, 0:2,'ShowText','on','LabelSpacing',1000);
caxis([-20 20])
colorbar
xlabel('$$y/L$$');
ylabel('$$z/H$$');

figure('units','centimeters','position',[10 10 .1*textwidth .5*textwidth]);
set(gcf,'PaperPositionMode','auto')
caxis([-20 20])
colorbar
axis off
end

function V = v(y,z,y0,z0,PSI,L,H)
    byl = y < y0;
    byu = y > y0;
    by = y == y0;
    bzl = z < z0;
    bzu = z > z0;
    V = PSI*(-byl.*bzl.*phi(y,y0).*dphi(z,z0)...
        + byl.*bzu.*phi(y,y0).*dphi(H-z,H-z0)...
        + byu.*bzu.*phi(L-y,L-y0).*dphi(H-z,H-z0)...
        - byu.*bzl.*phi(L-y,L-y0).*dphi(z,z0)...
        - by.*bzl.*dphi(z,z0)...
        + by.*bzu.*dphi(H-z,H-z0));
end

function W = w(y,z,y0,z0,PSI,L,H)
    byl = y < y0;
    byu = y > y0;
    bzl = z < z0;
    bzu = z > z0;
    bz = z == z0;
    W = PSI*(byl.*bzl.*dphi(y,y0).*phi(z,z0)...
        + byl.*bzu.*dphi(y,y0).*phi(H-z,H-z0)...
        - byu.*bzu.*dphi(L-y,L-y0).*phi(H-z,H-z0)...
        - byu.*bzl.*dphi(L-y,L-y0).*phi(z,z0)...
        + byl.*bz.*dphi(y,y0)...
        - byu.*bz.*dphi(L-y,L-y0));
end

function PHI = phi(x,x0)
    PHI = x.*(2.*x0-x)./(x0.^2);
end

function dPHI = dphi(x,x0)
    dPHI = 2.*(x0-x)./(x0.^2);
end

