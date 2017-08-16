%% p2b_case1
% IC : 20 000 particles uniformly distributed in compartment 1; 
%      no particle in compartment 2.
% close all;

fontsize = 12;
set(groot, 'defaultTextFontSize', fontsize);
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',18);

root = '/home/renaud/Documents/EPL/tfe/code/';
textwidth = 16.06;


t = 0:1000; %years
n = length(t);
C = cell(n,1);
C2 = cell(n,1);
C3 = cell(n,1);
outputdir = 'p2b_a75_2comp';
outputdir2 = 'p2b_a75_2comp_case2';
outputdir3 = 'p2b_a75_2comp_case3';
for i = 1:n
    name = ['Cbox' num2str(t(i))];
    C{i} = loadCbin(2,1,outputdir,name,'.out');
    C2{i} = loadCbin(2,1,outputdir2,name,'.out');
    C3{i} = loadCbin(2,1,outputdir3,name,'.out');
end

C_1 = zeros(1,n);
C2_1 = zeros(1,n);
C3_1 = zeros(1,n);
for i = 1:n
    C_1(i) = C{i}(1);
    C2_1(i) = C2{i}(1);
    C3_1(i) = C3{i}(1);
end
C2_1(C2_1 > 1) = 1;
Cmean = .5*(C{1}(1)+C{1}(2));
Cmean2 = .5*(C2{1}(1)+C2{1}(2));
Cmean3 = .5*(C3{1}(1)+C3{1}(2));
C0 = C{1};

alpha = GaussNewton(Cmean,C_1,t);
Ccomp = (C0(1)-Cmean)*exp(-2*alpha*t)+Cmean;

% A_1
Cdiscr = zeros(2,n);
Cdiscr(:,1) = C0;
A = [0.984 0.016; 0.016 0.984];
for i = 1:n-1
    Cdiscr(:,i+1) = A*Cdiscr(:,i);
end

% A_50 corrected
a50 = 0.877339744180938;
Cdiscr3 = zeros(2,n);
Cdiscr3(:,1) = C0;
A3 = [a50 1-a50; 1-a50 a50]^(1/50);
for i = 1:n-1
    Cdiscr3(:,i+1) = A3*Cdiscr3(:,i);
end

% (A_50)^(1/50) then corrected
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
plot(t,C_1/Cmean,'-k');
plot(t,Ccomp/Cmean,'--k');
plot(t,Cdiscr(1,:)/Cmean,':k');
plot(t(1:50:end),Cdiscr2(1,1:50:end)/Cmean,'-.k');
% plot(t(1:100:end),Cdiscr3(1,1:100:end)/Cmean,'-k');
h = legend('$$C_1^{num}(t)/\bar{C}$$','$$C_1^{cont}(t)/\bar{C}$$','$$C_1^{discr,1}(t)/\bar{C}$$','$$C_1^{discr,2}(t)/\bar{C}$$');
set(h,'FontSize',14); 
xlabel('$t$ [years]');
ylabel('[-]');

fig2 = figure('units','centimeters','position',[10 10 1*textwidth .5*textwidth]);
set(gcf,'PaperPositionMode','auto')
hold on;
plot(t,C_1/Cmean,'-k');
plot(t,C3_1/Cmean3,':k');
plot(t,C2_1/Cmean2,'--k');
h = legend('$$C_1^1(t)/\bar{C}$$','$$C_{1}^2(t)/\bar{C}$$','$$C_{1}^3(t)/\bar{C}$$');
set(h,'FontSize',14); 
xlabel('$t$ [years]');
ylabel('[-]');
ylim([.8 2]);