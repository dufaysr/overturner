fontsize = 12;
set(groot, 'defaultTextFontSize', fontsize);
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',fontsize);
set(groot, 'Units','centimeters');

%% import fil
close all
clear
bprint = 1;
binary = 0;
root = '/home/renaud/Documents/EPL/tfe/code/';
textwidth = 16.06;
model = 'testcase';

% version used for the report :
importfileInfo(model,'info',4,inf);
Y = load([root 'out/' model '/Y.out']);
Z = load([root 'out/' model '/Z.out']);
T = load([root 'out/' model '/time.out']); T =T(end);

ymin = min(Y); ymax = max(Y);
zmin = min(Z); zmax = max(Z);

assert(length(Y) == length(Z),'Y and Z must be the same length.');
nparticles = length(Y);

n = 20;
C = zeros(n,n);
dy = (ymax-ymin)/n;
dz = (zmax-zmin)/n;

for i=1:nparticles
    iy = min(floor((Y(i)-ymin)/dy),n-1)+1;
    iz = min(floor((Z(i)-zmin)/dz),n-1)+1;
    C(iz,iy) = C(iz,iy) + 1;
end