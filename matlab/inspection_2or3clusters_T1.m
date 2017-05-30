%% inspect if the 2 communities found for T = 10 should not be 3
clearvars
root = '/home/renaud/Documents/EPL/tfe/overturner/';
model = 'timmermans/0406';
name_info = 'info';
importfileInfo(model,name_info);
Nbox = nboxy*nboxz;
M = loadMbin(model,'M1');
C = zeros(nboxy, nboxz);
C2 = zeros(Nbox,1);
H = zeros(3,Nbox);
for iy = 1:nboxy
    for iz = 1:nboxz
        ind = (iy-1)*nboxz + iz;
        if (iz >= nboxz-1) || (iz == nboxz-2 && iy <= 3)
            H(3,ind) = 1;
            C(iy,iz) = 3;
            C2(ind) = 3;
        elseif (iz <= 2 && iy <= nboxy-2) || (iz == 3 && iy <= 5) || (iz == 4 && iy <= 3) || (iz == 5 && iy <= 2)
            H(1,ind) = 1;
            C(iy,iz) = 1;
            C2(ind) = 1;
        else
            H(2,ind) = 1;
            C(iy,iz) = 2;
            C2(ind) = 2;
        end
    end
end
% J = 10000;
Pi = ones(Nbox,1)./Nbox;
M50 = M^50;
rel = H*M*H';
prob_rel = diag(1./sum(rel,2))*rel;

fprintf('rel = \n'); disp(rel);
fprintf('prob_rel = \n'); disp(prob_rel);