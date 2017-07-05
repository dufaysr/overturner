function v2boxbis()
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
y0m = -1;
y0p = 1;
zstar = .75;
H = 1;

y = linspace(y0m,y0p);
z = linspace(zstar,H);
[Y,Z] = meshgrid(y,z);

figure;
plot(z,fz(z,zstar,H))

C = fy(Y,y0m,y0p).*fz(Z,zstar,H);
figure;
surf(Y,Z,C);

end

% function FY = fy(y,y0m,y0p)
%     FY = -(y-y0m).*(y-y0p);
% end

function FY = fy(y,y0m,y0p)
    FY = sin((2*pi/(y0p-y0m)).*(y-y0m)+1.5*pi) + 1;
end


% function FZ = fz(z,zstar,H)
%     FZ = sin((2*pi/(H-zstar)).*(z-zstar)+pi);
% end


function FZ = fz(z,zstar,H)
zm = (zstar+H)/2;
dz = (H-zstar)/2;
bzp = z >= zm;
bzm = z < zm;

FZ = -bzm.*(sin((2*pi/dz).*(z-zstar)+1.5*pi)+1)...
    + bzp.*(sin((2*pi/dz).*(z-zm)+1.5*pi)+1);
end