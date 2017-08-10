%% p2b_case1
% IC : 20 000 particles uniformly distributed in compartment 1; 
%      no particle in compartment 2.
close all;

fontsize = 12;
set(groot, 'defaultTextFontSize', fontsize);
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',18);

root = '/home/renaud/Documents/EPL/tfe/code/';
textwidth = 16.06;


t = 0:1000; %years
n = length(t);
C = cell(n,1);
outputdir = 'p2b_a75_2comp';
for i = 1:n
    name = ['Cbox' num2str(t(i))];
    C{i} = loadCbin(2,1,outputdir,name,'.out');
end

C1 = zeros(1,n);
for i = 1:n
    C1(i) = C{i}(1);
end

Cmean = .5*(C{1}(1)+C{1}(2));
C0 = C{1};
alpha = GaussNewton(Cmean,C1,t);
Ccomp = (C0(1)-Cmean)*exp(-2*alpha*t)+Cmean;

% A_1
Cdiscr = zeros(2,n);
Cdiscr(:,1) = C0;
A = [0.984 0.016; 0.016 0.984];
for i = 1:n-1
    Cdiscr(:,i+1) = A*Cdiscr(:,i);
end

% (A_50)^(1/50)
a = 0.997193285863386;
Cdiscr2 = zeros(2,n);
Cdiscr2(:,1) = C0;
A2 = [a 1-a; 1-a a];
for i = 1:n-1
    Cdiscr2(:,i+1) = A2*Cdiscr2(:,i);
end
%%
fig = figure('units','centimeters','position',[10 10 1*textwidth .5*textwidth]);
set(gcf,'PaperPositionMode','auto')
hold on;
plot(t,C1/Cmean,'-k');
plot(t,Ccomp/Cmean,'--k');
plot(t,Cdiscr(1,:)/Cmean,':k');
plot(t(1:100:end),Cdiscr2(1,1:100:end)/Cmean,'-.k');
h = legend('$$C_1^{num}(t)/\bar{C}$$','$$C_1^{cont}(t)/\bar{C}$$','$$C_1^{discr,1}(t)/\bar{C}$$','$$C_1^{discr,2}(t)/\bar{C}$$');
set(h,'FontSize',12); 
xlabel('$t$ [years]');
ylabel('[-]');