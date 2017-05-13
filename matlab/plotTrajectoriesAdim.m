% close all

Y = load('/home/renaud/Documents/EPL/tfe/overturner/out/model/Y.out');
Z = load('/home/renaud/Documents/EPL/tfe/overturner//out/model/Z.out');

L = 1;
H = 1;

figure
hold on
plot(Y,Z)
plot(Y(1,:),Z(1,:),'.g','MarkerSize',15)
plot(Y(end,:),Z(end,:),'.r','MarkerSize',15)
xlim([0 L]);
ylim([0 H]);