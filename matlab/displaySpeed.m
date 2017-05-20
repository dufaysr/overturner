close all;

Y = load('/home/renaud/Documents/EPL/tfe/overturner/out/Y.out');
Z = load('/home/renaud/Documents/EPL/tfe/overturner/out/Z.out');
V = load('/home/renaud/Documents/EPL/tfe/overturner/out/V.out');
W = load('/home/renaud/Documents/EPL/tfe/overturner/out/W.out');
figure;
contourf(Y,Z,V);
colorbar;

figure;
contourf(Y,Z,W,10,'Showtext','on');
colorbar;