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
H = zeros(5,Nbox);
for iy = 1:nboxy
    for iz = 1:nboxz
        ind = (iy-1)*nboxz + iz;
        if (iy <= 6 && iz <= 4) || (iy <= 5 && iz == 5)
            H(1,ind) = 1;
            C(iy,iz) = 1;
            C2(ind) = 1;
        elseif (iy >= 7 && iy <= 12 && iz <= 2) || (iz == 3 && iy >= 7 && iy <= 11)
            H(2,ind) = 1;
            C(iy,iz) = 2;
            C2(ind) = 2;
        elseif (iy >= 13) || (iy == 12 && iz >= 3) || (iy == 11 && iz >= 6) || (iy == 10 && iz >= 7) || (iy >= 8 && iz >= 8)  
            H(3,ind) = 1;
            C(iy,iz) = 3;
            C2(ind) = 3;
        elseif (iy <= 5 && iz >= 6) || (iy == 6 && iz >= 7) || (iy == 7 && iz >= 8)
            H(5,ind) = 1;
            C(iy,iz) = 5;
            C2(ind) = 5;
        else
            H(4,ind) = 1;
            C(iy,iz) = 4;
            C2(ind) = 4;
        end
    end
end

rel = H*M*H';
prob_rel = diag(1./sum(rel,2))*rel;

fprintf('rel = \n'); disp(rel);
fprintf('prob_rel = \n'); disp(prob_rel);