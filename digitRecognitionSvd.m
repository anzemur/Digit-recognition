function recog = digitRecognitionSvd(imagePath,data,singVals)
% digitRecognitionSvd(imagePath,data,singVals) - tries to recognize the digit from digitPath by solving the equation 
% Ui*Si*yi=b, where Ui is a matrix of left singular vectors from the SVD decomposition of the learning matrix A, U is the matrix of singular vectors
% and yi=V' * xi
% imagePath.. path to the image to test
% data.. containts matrices U and S for each number
% singVals.. how many singular values and vectors are to be used for the recognition

% Reads and creates the vector b (image column vector)
image = imread(imagePath);
imageColumn = double(convertToColumn(image));

nSingularVal = singVals;
norms = [];
for i=0:9
    s = zeros(size(data(i+1).s));
    s(1:nSingularVal,1:nSingularVal) = data(i+1).s(1:nSingularVal,1:nSingularVal);
    u = data(i+1).u;
    % Solve the equation
    y = (u*s)\imageColumn;
    res = norm(u'*imageColumn - s*y);
    norms = [norms,res];
end

% Find the minimum norm
[minval, col] = min(min(norms,[],1));
recog = col-1;
end
