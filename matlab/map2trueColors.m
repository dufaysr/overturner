function [CC] = map2trueColors(C,N,method)
[dimz,dimy] = size(C);
CC = zeros(dimz,dimy,3);
if nargin < 3
    method = 1;
end
if method == 1
    nr = ceil(N^(1/3));
    if nr*(nr-1)*(nr-1) >= N
        ng = nr-1;
        nb = nr-1;
    elseif nr*nr*(nr-1) >= N
        ng = nr;
        nb = nr-1;
    else
        ng = nr;
        nb = nr;
    end
    r = linspace(0,1,nr);
    g = linspace(0,1,ng);
    b = linspace(0,1,nb);
    
    colors = zeros(nr*ng*nb,3);
    ind = 1;
    for i = 1:nr
        for j = 1:ng
            for k = 1:nb
                colors(ind,:) = [r(i) g(j) b(k)];
                ind = ind + 1;
            end
        end
    end
    rng(0);
    colors = colors(randperm(nr*ng*nb),:);
    
    for j = 1:dimy
        for i = 1:dimz
            CC(i,j,:) = colors(C(i,j)+1,:);
        end
    end
else
    CC(:,:,1) = C./(N-1);
    
    rndom = zeros(2,N);
    rng(0);
    rndom(1,:) = randperm(N)/N;
    rng(1);
    rndom(2,:) = randperm(N)/N;
    
    for k = 1:2
        for j = 1:dimy
            for i = 1:dimz
                CC(i,j,k+1) = mod(CC(i,j,1)+rndom(k,C(i,j)+1),1);
            end
        end
    end
end
end