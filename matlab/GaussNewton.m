function alpha = GaussNewton(Cmean,Cvec,t)
% C0 = 2;
% Cmean = 1;
% alphareal = .5;
% t = linspace(0,10);
% Cvec = (C0-Cmean)*exp(-2*alphareal*t)+Cmean + .1*(2*rand(1,100)-1);

tol = 1e-9;
nmax = 1e3;

alpha = 0; % initial guess
S = Sfunc(alpha,Cvec,Cmean,t);
Sold = 0;
delta = S-Sold;
n = 1;

while ((abs(delta) > tol) && (n<nmax))
    J = Jfunc(alpha,Cvec,Cmean,t);
    r = rfunc(alpha,Cvec,Cmean,t);
    delta = (J*r')/(J*J');
    alpha = alpha - delta;
    n = n+1;
end

if (n >= nmax)
    fprintf(['Warning: maximal number of iterations reached.\n'...
        'Solution may be suboptimal.\n|delta| = %1.3e\n'],abs(delta));
else
    fprintf(['Optimal solution reached in %d iterations using GaussNewton.\n'...
        'alpha = %f with precision |delta| = %1.3e\n'],n,alpha,abs(delta));
    S = Sfunc(alpha,Cvec,Cmean,t);
    fprintf('S = %f\n\n',S);
    
    alpha_fms = fminsearch(@(alpha) Sfunc(alpha,Cvec,Cmean,t),0);
    S_fms = Sfunc(alpha_fms,Cvec,Cmean,t);
    fprintf('fminsearch find:\nalpha = %f\nS=%f.\n\n',alpha_fms,S_fms);
    
    if (S_fms < S)
        alpha = alpha_fms;
    end
end

end

%% auxiliary functions
function r = rfunc(alpha,Cvec,Cmean,t)
C0 = Cvec(1);
r = ((C0-Cmean)*exp(-2*alpha*t) + Cmean - Cvec).^2;
end

function S = Sfunc(alpha,Cvec,Cmean,t)
S = sum(rfunc(alpha,Cvec,Cmean,t));
end

function J = Jfunc(alpha,Cvec,Cmean,t)
C0 = Cvec(1);
J = -4*(C0-Cmean)*t.*exp(-2*alpha*t).*((C0-Cmean)*exp(-2*alpha*t)+Cmean-Cvec);
end