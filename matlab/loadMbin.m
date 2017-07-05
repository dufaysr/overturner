function [A] = loadMbin(model,name)
if nargin < 2
    name = 'global_box';
end
filename = ['/home/renaud/Documents/EPL/tfe/overturner/out/' model '/' name '.bin'];
fprintf('Trying to load ''%s''',filename);
f = fopen(filename);

A = fread(f,'double');
fclose(f);
n = sqrt(length(A));
A = reshape(A,n,n)';
end