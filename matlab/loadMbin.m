function [A] = loadMbin(outputdir,name)
if nargin < 2
    name = 'global_box';
end
filename = ['/home/renaud/Documents/EPL/tfe/code/out/' outputdir '/' name '.bin'];
fprintf('Trying to load ''%s''\n',filename);
f = fopen(filename);

A = fread(f,'double');
fclose(f);
n = sqrt(length(A));
A = reshape(A,n,n)';
end