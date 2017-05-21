% close all
% Y = load('/home/renaud/Documents/EPL/tfe/overturner/out/model/Y.out');
% Z = load('/home/renaud/Documents/EPL/tfe/overturner/out/model/Z.out');
% 
% Y2 = load('/home/renaud/Documents/EPL/tfe/overturner/out/Y2.out');
% Z2 = load('/home/renaud/Documents/EPL/tfe/overturner/out/Z2.out');
% L = 12e6;
% H = 4e3;

% Y = load('../out/timmermans/Y.out');
% Z = load('../out/timmermans/Z.out');
L = 15e6;
H = 5e3;

ind = 5:7;
figure
hold on
plot(Y(:,ind),Z(:,ind))
plot(Y(1,ind),Z(1,ind),'.g','MarkerSize',15)
plot(Y(end,ind),Z(end,ind),'.r','MarkerSize',15)
xlim([0 L]);
ylim([0 H]);
% xlim([0 1]);
% ylim([0 1]);


% figure
% hold on
% plot(Y2(:,1),Z2(:,1))
% plot(Y2(1,:),Z2(1,:),'.g','MarkerSize',15)
% plot(Y2(end,1),Z2(end,1),'.r','MarkerSize',15)
% xlim([0 L]);
% ylim([0 H]);