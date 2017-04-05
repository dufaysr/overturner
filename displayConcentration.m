function displayConcentration(filename)

C = load(filename);
x = linspace(0,1,size(C,1)); dx = x(2)-x(1);
y = linspace(0,1,size(C,2)); dy = y(2)-y(1);
[X,Y] = meshgrid(x,y);

figure
surf(X,Y,C,'FaceColor','texturemap')

int = dx*dy*sum(sum(C))
end

