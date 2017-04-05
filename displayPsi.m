function displayPsi(y0,z0)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
L = 1;
H = 1;
PSI = 1;

if nargin < 2
    y0 = .75;
    z0 = .75;
end

y = linspace(0,1);
z = y;
[Y,Z] = meshgrid(y,z);
Psi = psi(Y,Z);
surf(Y,Z,Psi,'LineStyle','none');


    function PHI = phi(x,x0)
        x(x > x0) = 0;
        PHI = x.*(2.*x0-x)./(x0.^2);           
    end
    function Psi = psi(y,z)
        Psi = .25*PSI*(phi(y,y0).*phi(z,z0) + phi(y,y0).*phi(H-z,H-z0)...
            + phi(L-y,L-y0).*phi(H-z,H-z0) + phi(L-y,L-y0).*phi(z,z0));
    end
end
