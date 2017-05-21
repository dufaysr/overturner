function importfileInfo(model, startRow, endRow)
%IMPORTFILE Import numeric data from a text file as column vectors.
%   [NAMES,VALUES] = IMPORTFILEINFO(MODEL) Reads data from text file
%   /home/renaud/Documents/EPL/tfe/overturner/out/MODEL/info.out
%   for the default selection STARTROW = 4, ENDROW = inf.
%
%   [NAMES,VALUES] = IMPORTFILEINFO(FILENAME, STARTROW, ENDROW) Reads data from
%   rows STARTROW through ENDROW of text file FILENAME.
%
% Example:
%   [names,values] = importfileInfo('model');


%% Initialize variables.
filename = ['/home/renaud/Documents/EPL/tfe/overturner/out/' model '/info.out'];
delimiter = {' = ',' '};
if nargin<=2
    startRow = 5;
    endRow = inf;
end

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
