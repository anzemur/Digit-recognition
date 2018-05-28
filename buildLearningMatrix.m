function A = buildLearningMatrix(path,number)
% buildLearningMatrix(path,number) - takes all the images from the path + number, converts their matrices
% to column vector and puts them side by side into the matrix
% Path example: "../digit-recognition/digits/1/"

path = strcat(strcat(strcat(path,'/'),num2str(number)),'/');
elCnt = length(dir(path)) - 2; % because of . and ..
A = [];


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
