%% p2b_case1
% IC : 20 000 particles uniformly distributed in compartment 1; 
%      no particle in compartment 2.
t = 0:100; %years
n = length(t);
C = cell(n,1);
outputdir = 'p2b_a75_2comp_2';
for i = 1:n
    name = ['Cbox' num2str(t(i))];
    C{i} = loadCbin(2,1,outputdir,name,'.out');
end

C1 = zeros(1,n);
for i = 1:n
    C1(i) = C{i}(1);
end
figure; hold on;
plot(t,C1);

Cmean = .5*(C{1}(1)+C{1}(2));
C0 = C1(1);
alpha = GaussNewton(Cmean,C1,t);
Ccomp = (C0-Cmean)*exp(-2*alpha*t)+Cmean;
plot(t,Ccomp);
