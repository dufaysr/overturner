function displaySpeed2box
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
textwidth = 16.06;
close all;

H = 1;
L = 1;
PSI = 1;

% y0 = 11/12;
% z0 = 3.5/4;
y0 = 13/15;
z0 = .75;

ytick = [-1 -1+y0 0 1-y0 1];
ztick = [0 z0 1];
yticklab = {'-1','$$\frac{y^-_0}{L}$$','0','$$\frac{y^+_0}{L}$$','1'};
zticklab = {'0','$$\frac{z_0}{H}$$','1'};

yl = linspace(-1,0,240);
yu = linspace(0,1,240);
y = linspace(-1,1,240);
z = linspace(0,1,100);
[YL,~] = meshgrid(yl,z);
[YU,~] = meshgrid(yu,z);
[Y,Z] = meshgrid(y,z);
V = v2box(Y,Z,y0,z0,PSI,L,H);
VL = v2box(YL,Z,y0,z0,PSI,L,H);
VU = v2box(YU,Z,y0,z0,PSI,L,H);
W = w2box(Y,Z,y0,z0,PSI,L,H);

figure('units','centimeters','position',[10 10 1.1*textwidth .5*textwidth]);
set(gcf,'PaperPositionMode','auto')
% set(gcf,'Resize','off')
set(gca,'xtick',ytick,'XTickLabel',yticklab)
set(gca,'ytick',ztick,'YTickLabel',zticklab)
hold on
plot(y,z0*ones(size(y)),':k');
plot((y0-1)*ones(size(z)),z,':k');
plot((1-y0)*ones(size(z)),z,':k');
contour(Y,Z,V);
colorbar

% contour(YU,Z,VU);
% contour(YL,Z,VL);
% contour(YU,Z,VU,1:2,'ShowText','on','LabelSpacing',150);
% contour(YU,Z,VU,[0 0],'ShowText','on','LabelSpacing',150);
% contour(YU,Z,VU, [-2 -2],'ShowText','on','LabelSpacing',90);
% contour(YU,Z,VU, [-4 -4],'ShowText','on','LabelSpacing',70);
% contour(YU,Z,VU, [-6 -6]);
% contour(YU,Z,VU, [-8 -8],'ShowText','on','LabelSpacing',1000);
% contour(YU,Z,VU, [-10 -12]);
% contour(YU,Z,VU, [-14 -14],'ShowText','on','LabelSpacing',1000);
% contour(YL,Z,VL,-2:-1,'ShowText','on','LabelSpacing',150);
% contour(YL,Z,VL,[0 0],'ShowText','on','LabelSpacing',150);
% contour(YL,Z,VL, [2 2],'ShowText','on','LabelSpacing',90);
% contour(YL,Z,VL, [4 4],'ShowText','on','LabelSpacing',70);
% contour(YL,Z,VL, [6 6]);
% contour(YL,Z,VL, [8 8],'ShowText','on','LabelSpacing',1000);
% contour(YL,Z,VL, [10 12]);
% contour(YL,Z,VL, [14 14],'ShowText','on','LabelSpacing',1000);
% caxis([-10 10])
% % colorbar
% xlabel('$$y/L$$');
% ylabel('$$z/H$$');

figure('units','centimeters','position',[10 10 1.1*textwidth .5*textwidth]);
set(gcf,'PaperPositionMode','auto')
% set(gcf,'Resize','off')
set(gca,'xtick',ytick,'XTickLabel',yticklab)
set(gca,'ytick',ztick,'YTickLabel',zticklab)
hold on
plot(y,z0*ones(size(y)),':k');
plot((y0-1)*ones(size(z)),z,':k');
plot((1-y0)*ones(size(z)),z,':k');
contour(Y,Z,W)
colorbar
% contour(Y,Z,W,-18:4:-2);
% contour(Y,Z,W,[-20 -20],'ShowText','on','LabelSpacing',1000);
% contour(Y,Z,W,[-10 -10],'ShowText','on','LabelSpacing',150);
% contour(Y,Z,W,[-4 -4],'ShowText','on','LabelSpacing',150);
% contour(Y,Z,W, 0:2,'ShowText','on','LabelSpacing',150);
% % contour(YU,Z,WU,-18:4:-2);
% % contour(YU,Z,WU,[-20 -20],'ShowText','on','LabelSpacing',1000);
% % contour(YU,Z,WU,[-10 -10],'ShowText','on','LabelSpacing',85.5);
% % contour(YU,Z,WU,[-4 -4],'ShowText','on','LabelSpacing',130);
% % contour(YU,Z,WU, 0:2,'ShowText','on','LabelSpacing',1000);
% caxis([-10 10])
% % colorbar
% xlabel('$$y/L$$');
% ylabel('$$z/H$$');

figure('units','centimeters','position',[10 10 .1*textwidth .5*textwidth]);
set(gcf,'PaperPositionMode','auto')
caxis([-10 10])
colorbar
axis off
end

function V2box = v2box(y,z,y0,z0,PSI,L,H)
    yl = y < 0;
    yu = y > 0;
    y0m = y0-L;
    y0p = L-y0;
    zstar = .75*H;
    bforcing = (y <= y0p) & (y >= y0m) & (z >= zstar) & (z <= H);
    V2box = yl.*v(y+L,z,y0,z0,PSI,L,H)+yu.*v(y,z,L-y0,z0,PSI,L,H)...
        + 2*bforcing.*fy(y,y0m,y0p).*fz(z,zstar,H);
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

function W2box = w2box(y,z,y0,z0,PSI,L,H)
    yl = y < 0;
    yu = y > 0;
    W2box = yl.*w(y+L,z,y0,z0,PSI,L,H) + yu.*w(y,z,L-y0,z0,PSI,L,H);
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

% function FY = fy(y,y0m,y0p)
%     FY = -(y-y0m).*(y-y0p);
% end
% 
% function FZ = fz(z,zstar,H)
%     FZ = sin((2*pi/(H-zstar)).*(z-zstar)+pi);
% end

function FY = fy(y,y0m,y0p)
    FY = sin((2*pi/(y0p-y0m)).*(y-y0m)+1.5*pi) + 1;
end

function FZ = fz(z,zstar,H)
zm = (zstar+H)/2;
dz = (H-zstar)/2;
bzp = z >= zm;
bzm = z < zm;

FZ = -bzm.*(sin((2*pi/dz).*(z-zstar)+1.5*pi)+1)...
    + bzp.*(sin((2*pi/dz).*(z-zm)+1.5*pi)+1);
end