Yadim = load('/home/renaud/Documents/EPL/tfe/overturner/out/model/Y.out');
Zadim = load('/home/renaud/Documents/EPL/tfe/overturner/out/model/Z.out');
tadim = load('/home/renaud/Documents/EPL/tfe/overturner/out/model/time.out');

Ydim1 = load('/home/renaud/Documents/EPL/tfe/overturner/out/modeldim1/Y.out');
Zdim1 = load('/home/renaud/Documents/EPL/tfe/overturner/out/modeldim1/Z.out');
tdim1 = load('/home/renaud/Documents/EPL/tfe/overturner/out/modeldim1/time.out');

Ydim2 = load('/home/renaud/Documents/EPL/tfe/overturner/out/modeldim2/Y.out');
Zdim2 = load('/home/renaud/Documents/EPL/tfe/overturner/out/modeldim2/Z.out');
tdim2 = load('/home/renaud/Documents/EPL/tfe/overturner/out/modeldim2/time.out');

figure
hold on
plot(tadim*L*H/Psi,Yadim(:,1)*L,tdim1(1:75),Ydim1(1:75,1));
legend('Adim','Dim');

figure
hold on
plot(tadim*L*H/Psi,Zadim(:,1)*H,tdim1(1:75),Zdim1(1:75,1));
legend('Adim','Dim')
