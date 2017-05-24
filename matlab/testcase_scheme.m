%% Used to generate figures in section test case
fontsize = 20;
set(groot, 'defaultTextInterpreter', 'latex'); 
set(groot, 'defaultAxesTickLabelInterpreter','latex');
set(groot, 'defaultLegendInterpreter','latex');
set(groot, 'defaultTextFontSize', fontsize); 
set(groot, 'defaultAxesFontSize',fontsize)
set(groot, 'defaultLegendFontSize',fontsize);

close all;
x = linspace(0,1);
s = size(x);
y1 = .3;
z1 = .7;
y = y1*ones(s);
z = z1*ones(s);

for i = 1:2
figure1 = figure;
axes1 = axes('Parent',figure1);
hold(axes1,'on');
plot3(zeros(s),y1*ones(s),linspace(0,z1),':k');
plot3(zeros(s),linspace(0,y1),z1*ones(s),':k');
plot3(x,y,z,'--k');
plot3(x(1),y1,z1,'.k','MarkerSize',15);
plot3(x(end),y1,z1,'.k','MarkerSize',15);
xlabel('$x$');
ylabel('$y$');
zlabel('$z$');
xlim([0 1]);
ylim([0 1]);
zlim([0 1]);
box(axes1,'on');
% set(axes1,'DataAspectRatio',[1 1 1],'XTick',[0 1],'XTickLabel',{'0','$B$'},...
%     'YTick',[0 0.3 1],'YTickLabel',{'0','$y_0$','$L$'},'ZTick',[0 0.7 1],...
%     'ZTickLabel',{'0','$z_0$','$H$'});
set(axes1,'DataAspectRatio',[1 1 1],'XTick',[0 1],'XTickLabel',{'0','$B$'},...
    'YTick',[0.3],'YTickLabel',{'$y_0$'},'ZTick',[0.7],...
    'ZTickLabel',{'$z_0$'});
if i == 1
    view(axes1,[69.6 -21.2]);
    annotation(figure1,'textbox',...
    [0.4 0.56 0.029 0.057],...
    'String',{'$Q$'}, 'interpreter', 'latex',...
    'LineStyle','none');
%     matlab2tikz('/home/renaud/Documents/EPL/tfe/latex/inputs/testcase_scheme3D.tex','.5\textwidth');
else
    view(axes1,[90 0]);
    annotation(figure1,'textbox',...
    [0.4 0.663244047619046 0.029 0.0570000000000001],... % [0.445295857015804 0.663244047619046 0.029 0.0570000000000001]
    'String',{'$Q$'}, 'interpreter', 'latex',...
    'LineStyle','none');
%     matlab2tikz('imagesAsPng',true);
%     matlab2tikz('/home/renaud/Documents/EPL/tfe/latex/inputs/testcase_scheme2D.tex','width','.5\textwidth');
end
end