% close all

Y = load('out/Y.out');
Z = load('out/Z.out');

L = 12e6;
H = 4e3;

figure
hold on
plot(Y,Z)
plot(Y(1,:),Z(1,:),'.g','MarkerSize',15)
plot(Y(end,:),Z(end,:),'.r','MarkerSize',15)
xlim([0 L]);
ylim([0 H]);