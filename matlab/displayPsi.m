function displayPsi(y0,z0)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
textwidth = 16.06;
close all;

H = 1;
L = 1;
PSI = 1;

if nargin < 2
    y0 = 11/12;
    z0 = 3.5/4;
end

ytick = [0 y0 1];
ztick = [0 z0 1];
yticklab = {'0','$$\frac{y_0}{L}$$','1'};
zticklab = {'0','$$\frac{z_0}{H}$$','1'};

y = linspace(0,1,120);
z = linspace(0,1,100);
[Y,Z] = meshgrid(y,z);
Psi = psi(Y,Z);

% fig = figure('units','centimeters','position',[10 10 textwidth textwidth]);
% set(gcf,'PaperPositionMode','auto')
% set(gcf,'Resize','off')
% set(gca,'xtick',ytick,'XTickLabel',yticklab)
% set(gca,'ytick',ztick,'YTickLabel',zticklab)
% hold on
% % contour(Y,Z,Psi,15)
% % contour(Y,Z,Psi,[0 0])
% surf(Y,Z,Psi,'LineStyle','none');
% [~,h] = contour(Y,Z,Psi,0:.1:1,'ShowText','on','TextStep',.5,'LabelSpacing',3000);
% h.ContourZLevel = -1.5;

figure('units','centimeters','position',[10 10 .7*textwidth .7*textwidth]);
set(gcf,'PaperPositionMode','auto')
% set(gcf,'Resize','off')
set(gca,'xtick',ytick,'XTickLabel',yticklab)
set(gca,'ytick',ztick,'YTickLabel',zticklab)
hold on
plot(y,z0*ones(size(y)),':k');
plot(y0*ones(size(z)),z,':k');
contour(Y,Z,Psi,0:.1:1,'ShowText','on','LabelSpacing',3000);
caxis([0 1.2])
xlabel('$$y/L$$');
% xlabel('$$\frac{y}{L}$$');
ylabel('$$z/H$$');
% ylabel('$$\frac{z}{H}$$');
% colormap(gray)






    function PHI = phi(x,x0)
        PHI = x.*(2.*x0-x)./(x0.^2);
    end

    function Psi = psi(y,z)
        byl = y < y0;
        byu = y > y0;
        by = y == y0;
        bzl = z < z0;
        bzu = z > z0;
        bz = z == z0;
        Psi = PSI*(byl.*bzl.*phi(y,y0).*phi(z,z0)...
            + byl.*bzu.*phi(y,y0).*phi(H-z,H-z0)...
            + byu.*bzu.*phi(L-y,L-y0).*phi(H-z,H-z0)...
            + byu.*bzl.*phi(L-y,L-y0).*phi(z,z0)...
            + by.*bzl.*phi(z,z0)...
            + by.*bzu.*phi(H-z,H-z0)...
            + byl.*bz.*phi(y,y0)...
            + byu.*bz.*phi(L-y,L-y0)...
            + by.*bz);
    end
end
