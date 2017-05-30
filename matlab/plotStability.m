close all;
% Name of the model
model = 'timmermans';
% name of the files
name_info = 'info_globalbox';
nameM = 'M0';
% Load transition probability matrix from binary output file
fprintf('Loading Transition probabilities matrix...\n');
M = loadMbin(model,nameM);
% Import model parameters in the workspace
fprintf('Importing model parameters...\n');
importfileInfo(model,name_info);
% Compute clustering using stability criterion at Markov time 1, which is
% equivalent to modularity. We are not interested in the variation of
% information (hence 'noVI' option) and the graph is directed.
fprintf('Computing clustering...\n');
T = linspace(0,10,101); T(1) = 0.001;
% T = 1;
[S, N, VI, C] = stability(M,T,'directed','v','plot');
fprintf('Found %d communities with stability = %f.\n',N,S);
% Node k corresponds to spatial position iy = k/dimz, iz = mod(k,dimz)
ind = find(T == 1);
C = reshape(C(:,ind),nboxz,nboxy);
disp(C);
L = L1-L0;
H = H1-H0;
dy = L/dimy; dz = H/dimz;
y = linspace(dy/2,L-dy/2,dimy);
z = linspace(dz/2,H-dz/2,dimz);

%%
figure;
hold on
imagesc(y,z,C);
set(gca, 'YDir', 'normal');
% colormap(lines);
colormap(colorcube);
plot(linspace(0,L),ones(1,100)*z0,'--r');
plot(ones(1,100)*y0,linspace(0,H),'--r');
xlabel('y');
ylabel('z');
%% works too
% CC = map2trueColors(C,N);
% figure;
% image(CC);