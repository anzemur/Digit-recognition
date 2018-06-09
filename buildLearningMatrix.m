function A = buildLearningMatrix(path,number)
% buildLearningMatrix(path,number) - takes all the images from the path + number, converts their matrices
% to column vector and puts them side by side into the matrix that will be used in the algorithms
% path... path to the map where there is a map named training and contains examples for a certain number
% A... returns a matrix of which columns are images

% Create the path to the training examples
path = strcat(strcat(strcat(path,'/'),num2str(number)),'/training/');

% Count the number of elements in the folder
elCnt = length(dir(path)) - 2; % because of . and ..
A = [];

% Iterate through images, convert them to columns and add them to the A matrix
for i = 0 : elCnt - 1
    imgPath = strcat(strcat(path,num2str(i)),'.jpeg');
    number = imread(imgPath);
    numSize = length(number);
    numMat = [];
    for j = 1 : numSize
        numMat = [numMat;number(:,j)];
    end
    A(:,i+1) = numMat;

end

end  % function
