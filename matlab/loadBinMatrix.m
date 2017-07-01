function [A] = loadBinMatrix(name,dim1,dim2,path)
if nargin < 4
    path = '/home/renaud/Documents/EPL/tfe/overturner/out/';
end
disp([path name '.bin']);
f = fopen([path name '.bin']);

A = fread(f,'double');
fclose(f);
A = reshape(A,dim2,dim1)';
end