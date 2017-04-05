close all;

Y = load('out/Y.out');
Z = load('out/Z.out');
V = load('out/V.out');
W = load('out/W.out');
figure;
contourf(Y,Z,V);
colorbar;

figure;
contourf(Y,Z,W,10,'Showtext','on');
colorbar;