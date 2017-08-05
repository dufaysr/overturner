function [A] = loadCbin(nboxy,nboxz,outputdir,name,ext)
if nargin < 3
    ext = '.bin';
end
filename = ['/home/renaud/Documents/EPL/tfe/code/out/' outputdir '/' name ext];
fprintf('Trying to load ''%s''\n',filename);
f = fopen(filename);

A = fread(f,'double');
fclose(f);
A = reshape(A,nboxz,nboxy)';
end