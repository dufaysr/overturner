fig = figure('units','centimeters','position',[10 10 .5*textwidth .5*textwidth]);

set(gcf,'PaperPositionMode','auto')
set(gcf,'Resize','off')
%     set(gcf,'Visible', 'off'); % Prevent figure from popping up
set(gca,'xtick',ytick,'XTickLabel',yticklab)
set(gca,'ytick',ztick,'YTickLabel',zticklab)
hold on
grid on
imagesc(y,z,Cloc);
set(gca, 'YDir', 'normal');
% colormap(lines); % other colormap option
colormap(colorcube);
plot(ygrid,zygrid,'--k');
plot(yzgrid,zgrid,'--k');
plot(linspace(0,L),ones(1,100)*z0,'--k','linewidth',2);
plot(ones(1,100)*y0,linspace(0,H),'--k','linewidth',2);
plot(y0,z0,'.k','MarkerSize',30);
xlim([0 L1-L0]);
ylim([0 H1-H0]);
xlabel('y');
ylabel('z');