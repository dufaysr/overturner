function importfileInfo(model, name, startRow, endRow)
%IMPORTFILEINFO Import numeric data from a text file and store them in the
%workspace.
%   IMPORTFILEINFO(MODEL) Reads data from text file
%   /home/renaud/Documents/EPL/tfe/overturner/out/MODEL/info.out
%   for the default selection STARTROW = 4, ENDROW = inf.
%
%   IMPORTFILEINFO(FILENAME, STARTROW, ENDROW) Reads data from
%   rows STARTROW through ENDROW of text file FILENAME.
%
% Example:
%   importfileInfo('model');


%% Initialize variables.
delimiter = {' = ',' '};
if nargin < 4
    endRow = inf;
    if nargin < 3
        startRow = 5;
        if nargin < 2
            name = 'info';
        end
    end
end

filename = ['/home/renaud/Documents/EPL/tfe/code/out/' model '/' name '.out'];

%% Format for each line of text:
%   column2: text (%s)
%	column3: double (%f)
% For more information, see the TEXTSCAN documentation.
formatSpec = '%s%f%[^\n\r]';

%% Open the text file.
fileID = fopen(filename,'r');

%% Read columns of data according to the format.
% This call is based on the structure of the file used to generate this
% code. If an error occurs for a different file, try regenerating the code
% from the Import Tool.
dataArray = textscan(fileID, formatSpec, endRow(1)-startRow(1)+1,...
    'Delimiter', delimiter, 'HeaderLines', startRow(1)-1, 'ReturnOnError',...
    false, 'EndOfLine', '\r\n');
for block=2:length(startRow)
    frewind(fileID);
    dataArrayBlock = textscan(fileID, formatSpec,...
        endRow(block)-startRow(block)+1, 'Delimiter', delimiter,...
        'HeaderLines', startRow(block)-1, 'ReturnOnError', false,...
        'EndOfLine', '\r\n');
    for col=1:length(dataArray)
        dataArray{col} = [dataArray{col};dataArrayBlock{col}];
    end
end

%% Close the text file.
fclose(fileID);

%% Allocate imported array to column variable names
names = dataArray{:, 1};
values = dataArray{:, 2};

for i = 1:size(values,1)
    assignin('base',names{i},values(i));
end
end
