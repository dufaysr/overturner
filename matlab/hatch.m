function [X,Y] = hatch(ll,ur,dl,dec)
%   ---------------ur
%   |               |
%   |               |
%   ll---------------
a = (ur(2)-ll(2))/(dec*dl);
th = ll(1):dl:ur(1); nth = length(th);
if a > 0
    tv = ll(2):a*dl:ur(2);
else
    tv = ur(2):a*dl:ll(2);
end
ntv = length(tv);
n = nth + ntv;
X = zeros(n,2);
Y = zeros(n,2);

if a > 0
    X(1:ntv,1) = ll(1);
    X(ntv+1:n,1) = th;
    Y(1:ntv,1) = tv;
    Y(ntv+1:n,1) = ll(2);
    X(:,2) = X(:,1)+(ur(2)-Y(:,1))/a;
    ind = X(:,2) > ur(1);
    X(ind,2) = ur(1);
    Y(:,2) = a*(X(:,2)-X(:,1)) + Y(:,1);
else
    X(1:nth,1) = th;
    X(nth+1:n,1) = ll(1);
    Y(1:nth,1) = ur(2);
    Y(nth+1:n,1) = tv;
    X(:,2) = X(:,1)+(ll(2)-Y(:,1))/a;
    ind = X(:,2) > ur(1);
    X(ind,2) = ur(1);
    Y(:,2) = a*(X(:,2)-X(:,1)) + Y(:,1); 
end
X = X';
Y = Y';
end