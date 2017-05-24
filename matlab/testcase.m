fontsize = 12;
set(groot, 'defaultTextFontSize', fontsize); 
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',fontsize);
set(groot, 'Units','centimeters');

%% import fil
close all
clear
binary = 0;
root = '/home/renaud/Documents/EPL/tfe/overturner/';
textwidth = 16.06;
model = 'testcaseSI';

% importfileInfo(model,4,inf);
% Y = load([root 'out/' model '/Y.out']);
% Z = load([root 'out/' model '/Z.out']);
% T = load([root 'out/' model '/time.out']); T =T(end);

% version used for the report :
importfileInfo(model,4,inf,1);
Y = load([root 'out/' model '/Y_report.out']);
Z = load([root 'out/' model '/Z_report.out']);
T = load([root 'out/' model '/time_report.out']); T =T(end);

ymin = min(Y); ymax = max(Y);
zmin = min(Z); zmax = max(Z);

assert(length(Y) == length(Z),'Y and Z must be the same length.');
nparticles = length(Y);

n = 20;
C = zeros(n,n);
dy = (ymax-ymin)/n;
dz = (zmax-zmin)/n;

for i=1:nparticles
    iy = min(floor((Y(i)-ymin)/dy),n-1)+1;
    iz = min(floor((Z(i)-zmin)/dz),n-1)+1;
    C(iz,iy) = C(iz,iy) + 1;
end
C = C/nparticles; % C(i,j) correspond to the box [(i-1)dz, idz] x [(j-1)dy, jdy]
y = linspace(ymin,ymax,n);
z = linspace(zmin,zmax,n);

sv = V*T;
sw = W*T;
Cexact = zeros(n,n);
nm = 20;
Ctmp = zeros(nm,nm);
if strcmp(model,'testcase')
    fprintf('Infinite domain :\n\n');
    for iy = 1:n
        ym = linspace(ymin+(iy-1)*dy,ymin+iy*dy,nm+1);
        ym = ym(1:end-1);
        for iz = 1:n
            zm = linspace(zmin+(iz-1)*dz,zmin+iz*dz,nm+1);
            zm = zm(1:end-1);
            [YM,ZM] = meshgrid(ym,zm);
            Ctmp = J*exp(-((YM-y1-sv).^2)/(4*Kyy*T)-((ZM-z1-sw).^2)/(4*Kzz*T))/(4*pi*T*sqrt(Kyy*Kzz));
            Cexact(iz,iy) = sum(sum(Ctmp))/(nm*nm);
        end
    end
    Cexact = Cexact/sum(sum(Cexact));
elseif strcmp(model,'testcaseSI')
    fprintf('Semi-Infinite domain :\n\n');
    for iy = 1:n
        ym = linspace(ymin+(iy-1)*dy,ymin+iy*dy,nm+1);
        ym = ym(1:end-1);
        for iz = 1:n
            zm = linspace(zmin+(iz-1)*dz,zmin+iz*dz,nm+1);
            zm = zm(1:end-1);
            [YM,ZM] = meshgrid(ym,zm);
            Ctmp = J*exp(-((YM-y1-sv).^2)/(4*Kyy*T)).*(exp(-((ZM-z1).^2)/(4*Kzz*T))+exp(-((ZM+z1).^2)/(4*Kzz*T)))/(4*pi*T*sqrt(Kyy*Kzz));
            Cexact(iz,iy) = sum(sum(Ctmp))/(nm*nm);
        end
    end
    Cexact = Cexact/sum(sum(Cexact));
end
    
% if strcmp(model,'testcase')
%     Cexact = J*exp(-((YY-y1-sv).^2)/(4*Kyy*T)-((ZZ-z1-sw).^2)/(4*Kzz*T))/(4*pi*T*sqrt(Kyy*Kzz));
%     Cexact = Cexact/sum(sum(Cexact));
% elseif strcmp(model,'testcaseSI')
%     fprintf('Semi-Infinite domain :\n\n');
%     Cexact = J*exp(-((YY-y1-sv).^2)/(4*Kyy*T)).*(exp(-((ZZ-z1).^2)/(4*Kzz*T))+exp(-((ZZ+z1).^2)/(4*Kzz*T)))/(4*pi*T*sqrt(Kyy*Kzz));
%     Cexact = Cexact/sum(sum(Cexact));
% end
fprintf('Integral(C) = %1.3e\n',sum(sum(C)));
fprintf('Integral(Cexact) = %1.3e\n\n',sum(sum(Cexact)));
fprintf('||Cexact-C||inf = %1.3e\n',max(max(abs(Cexact-C))));
if strcmp(model,'testcaseSI')
    fprintf('||Cexact(y,0)-C(y,0)||inf = %1.3e\n',max(max(abs(Cexact(1,:)-C(1,:)))));
end

% load('C_25000.mat')
% load('C_50000_dt3600.mat');

fig0 = figure('units','centimeters','position',[10 10 textwidth .8*textwidth]);
set(gcf,'PaperPositionMode','auto')
set(gcf,'Resize','off')
hold on;
if strcmp(model,'testcaseSI')
    surf([-10e5 y(2:end)],[0 z(2:end)],C,'FaceColor',[0 0 0],'FaceAlpha',.5,'EdgeColor','none');
    surf([-10e5 y(2:end)],[0 z(2:end)],Cexact,'FaceColor',[1 1 1],'FaceAlpha',.5);
else
    surf(y,z,C,'FaceColor',[0 0 0],'FaceAlpha',.5,'EdgeColor','none');
    surf(y,z,Cexact,'FaceColor',[1 1 1],'FaceAlpha',.5);
end
xlabel('$y$'); ylabel('$z$'); zlabel('$x$');
axis tight
% legend('Numeric','Analytic');
% Diagnostics
yy = linspace(ymin,ymax-dy,n);
zz = linspace(zmin,zmax-dz,n);
[YY,ZZ] = meshgrid(yy,zz);
% mass center
rnum = [sum((yy+dy/2).*sum(C,1)) (zz+dz/2)*sum(C,2)];
irnumy = min(floor((rnum(1)-ymin)/dy),n-1)+1;
irnumz = min(floor((rnum(2)-zmin)/dz),n-1)+1;
if strcmp(model,'testcaseSI')
%     rexact = [sum((yy+dy/2).*sum(Cexact,1)) (zz+dz/2)*sum(Cexact,2)];
    rexact = [y1+V*T (zz+dz/2)*sum(Cexact,2)];
    iry  = min(floor((rexact(1)-ymin)/dy),n-1)+1;
    irz = min(floor((rexact(2)-zmin)/dz),n-1)+1;
else
    rexact = [y1+V*T z1+W*T]; % analytic, in the case of an infinite domain
    iry = min(floor((rexact(1)-ymin)/dy),n-1)+1;
    irz = min(floor((rexact(2)-zmin)/dz),n-1)+1;
end    
plot3(rexact(1),rexact(2),Cexact(irz,iry),'o','MarkerSize',10,'MarkerEdgeColor','k','MarkerFaceColor',[1,1,1]);
plot3(rnum(1),rnum(2),Cexact(irnumz,irnumy),'kx','MarkerSize',20,'linewidth',3)
legend('$C_{num}$','$C_{exact}$','Analytic center of mass','Numeric center of mass');
Erel_r = abs((rnum-rexact)./rexact);
fprintf('Center of mass analytic : (%1.3e,%1.3e)\n',rexact(1),rexact(2));
fprintf('Center of mass numeric : (%1.3e,%1.3e)\n',rnum(1),rnum(2));
fprintf('Relative error for center of mass : (%1.3e, %1.3e).\nNorm : %1.3e\n\n',...
        Erel_r(1),Erel_r(2),norm(Erel_r));
view([134 38.8]);
if strcmp(model,'testcaseSI')
    print(fig0,'/home/renaud/Documents/EPL/tfe/latex/fig/testcase/testcaseSI_surf.eps','-depsc2','-loose');
else
    print(fig0,'/home/renaud/Documents/EPL/tfe/latex/fig/testcase/testcase_surf.eps','-depsc2','-loose');
end
% close(fig0)
    
% variance
sigmanum = sum(sum(((YY+dy/2-rnum(1)).^2+(ZZ+dz/2-rnum(2)).^2).*C));
if strcmp(model,'testcaseSI')
    sigmaexact = sum(sum(((YY+dy/2-rexact(1)).^2+(ZZ+dz/2-rexact(2)).^2).*Cexact));
else
    sigmaexact = 2*(Kyy+Kzz)*T;
end
Erel_sigma = abs(sigmanum-sigmaexact)/sigmaexact;
fprintf('Variance analytic : %1.3e\n',sigmaexact);
fprintf('Variance numeric : %1.3e\n',sigmanum);
fprintf('Relative error for variance : %1.3e\n\n', Erel_sigma);

% 2D cuts
% fix y at iry
fig1 = figure('units','centimeters','position',[10 10 .5*textwidth .4*textwidth]);
set(gcf,'PaperPositionMode','auto')
set(gcf,'Resize','off')
plot(z,C(:,iry),'--k',z,Cexact(:,iry),'-k');
% legend('$C(y_1+VT,z)$','$C_{a}(y_1+VT,z)$');
xlabel('z');
ylabel('C');
if strcmp(model,'testcaseSI')
    print(fig1,'/home/renaud/Documents/EPL/tfe/latex/fig/testcase/testcaseSI_fixedy.eps','-depsc2','-loose');
else
    print(fig1,'/home/renaud/Documents/EPL/tfe/latex/fig/testcase/testcase_fixedy.eps','-depsc2','-loose');
end
close(fig1)

fig2 = figure('units','centimeters','position',[10 10 .5*textwidth .4*textwidth]);
set(gcf,'PaperPositionMode','auto')
set(gcf,'Resize','off')
if strcmp(model,'testcaseSI')
    plot(y,C(1,:),'--k',y,Cexact(1,:),'-k');
else
    plot(y,C(irz,:),'--k',y,Cexact(irz,:),'-k');
end
% legend('$C(y,z_1+WT)$','$C_{a}(y,z_1+WT)$');
xlabel('y');
ylabel('C');
if strcmp(model,'testcaseSI')
    print(fig2,'/home/renaud/Documents/EPL/tfe/latex/fig/testcase/testcaseSI_fixedz.eps','-depsc2','-loose'); 
else
    print(fig2,'/home/renaud/Documents/EPL/tfe/latex/fig/testcase/testcase_fixedz.eps','-depsc2','-loose');
end
close(fig2)