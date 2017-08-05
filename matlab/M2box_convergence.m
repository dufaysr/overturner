outputdir = 'problem2box_a75';

M1_5000a = loadMbin(outputdir,'M1_5000a');
M1_5000b = loadMbin(outputdir,'M1_5000b');
norm(M1_5000a-M1_5000b,'fro')

M1_10000a = loadMbin(outputdir,'M1_10000a');
M1_10000b = loadMbin(outputdir,'M1_10000b');
norm(M1_10000a-M1_10000b,'fro')

M1_20000a = loadMbin(outputdir,'M1_20000a');
M1_20000b = loadMbin(outputdir,'M1_20000b');
norm(M1_20000a-M1_20000b,'fro')

M1_40000a = loadMbin(outputdir,'M1_40000a');
M1_40000b = loadMbin(outputdir,'M1_40000b');
norm(M1_40000a-M1_40000b,'fro')

M1_90000a = loadMbin(outputdir,'M1_90000a');
M1_90000b = loadMbin(outputdir,'M1_90000b');
norm(M1_90000a-M1_90000b,'fro')

M1 = loadMbin(outputdir,'M1_1500000');
H = zeros(300,2);
H(1:150,1) = 1;
H(151:300,2) = 1;
D = diag(ones(300,1)./150);
M1_1500000 = H'*M1*D*H;

T = [1 10:10:100];
nT = length(T);
M = cell(nT,1);
M{1} = M1_10000a;
% M{1} = M1_1500000;
M{2} = loadMbin(outputdir,'M10_10000');
M{3} = loadMbin(outputdir,'M20_10000');
M{4} = loadMbin(outputdir,'M30_10000');
M{5} = loadMbin(outputdir,'M40_10000');
M{6} = loadMbin(outputdir,'M50_10000');
M{7} = loadMbin(outputdir,'M60_10000');
M{8} = loadMbin(outputdir,'M70_10000');
M{9} = loadMbin(outputdir,'M80_10000');
M{10} = loadMbin(outputdir,'M90_10000');
M{11} = loadMbin(outputdir,'M100_10000');
M{1} = M{6}^(1/50);
Mbis = M;

for i = 1:nT
    for j = 1:i
       Mbis{j} = M{j}^(T(i)/T(j)); 
    end
    for k=1:2
        for j = 1:i
            fprintf('|%1.3f, %1.3f|\t',Mbis{j}(k,1),Mbis{j}(k,2));
        end
        fprintf('\n');
    end
    fprintf('\n');
end