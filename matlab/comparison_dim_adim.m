close all
model = 'timmermans';
% importfileInfo(model);
Psi = 2;
L = 15e6;
H = 5e3;
Yadim = load(['/home/renaud/Documents/EPL/tfe/overturner/out/' model '/Yadim.out']);
Zadim = load(['/home/renaud/Documents/EPL/tfe/overturner/out/' model '/Zadim.out']);
tadim = load(['/home/renaud/Documents/EPL/tfe/overturner/out/' model '/tadim.out']);

Yadim2 = load(['/home/renaud/Bureau/overturner/out/' model '/Yadim.out']);
Zadim2 = load(['/home/renaud/Bureau/overturner/out/' model '/Zadim.out']);
tadim2 = load(['/home/renaud/Bureau/overturner/out/' model '/tadim.out']);

Ydim = load(['/home/renaud/Documents/EPL/tfe/overturner/out/' model '/Y.out']);
Zdim = load(['/home/renaud/Documents/EPL/tfe/overturner/out/' model '/Z.out']);
tdim = load(['/home/renaud/Documents/EPL/tfe/overturner/out/' model '/time.out']);
 
Ydim2 = load(['/home/renaud/Bureau/overturner/out/' model '/Y.out']);
Zdim2 = load(['/home/renaud/Bureau/overturner/out/' model '/Z.out']);
tdim2 = load(['/home/renaud/Bureau/overturner/out/' model '/time.out']);

figure
hold on
% plot(tadim*L*H/Psi,Yadim(:,1)*L,tdim,Ydim(:,1));
plot(tdim2,Ydim2(:,1),tdim,Ydim(:,1),tadim2*L*H/Psi,Yadim2(:,1)*L, tadim*L*H/Psi,Yadim(:,1)*L);
legend('dim2','Dim','ADim2','Adim');

figure
hold on
% plot(tadim*L*H/Psi,Zdim2(:,1)*H,tdim,Zdim(:,1));
plot(tdim2,Zdim2(:,1),tdim,Zdim(:,1),tadim2*L*H/Psi,Zadim2(:,1)*H, tadim*L*H/Psi,Zadim(:,1)*H);
legend('dim2','Dim','Adim2','adim')
% %%
% figure;
% plot(Yadim,Zadim)
% 
figure;
plot(Ydim,Zdim,L*Yadim,H*Zadim,Ydim2,Zdim2,L*Yadim2,H*Zadim2)
legend('dim','adim','dim2','adim2');
ylim([0 H])
xlim([0 L])
