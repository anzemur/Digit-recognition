function recog = digitRecognitionLeastSquares(imagePath,data,singVals)
% digitRecognitionLeastSquares(imagePath,data,singVals) - tries to recognize the digit from digitPath by solving the equation 
% Ai*xi=b, where Ai is a learnign matrix for digit 0..9, b is a column vector of an image. It does so using the least squares method. 
% Solution is the number which has the smallest norm(b - Ai*xi)
% imagePath.. path to the image to test
% data .. containts learning matrix A for each number
% singVals .. useless argument, to have the same function signature for testing purposes

% Reads and creates the vector b (image column vector)
image = imread(imagePath);
imageColumn = double(convertToColumn(image));

norms = [];
for i=0 : 9
    a = data(i+1).a;
    % Find the solution using the least squares method and calculates the norm
    x = a\imageColumn;
    norms = [norms,norm(imageColumn - a*x)];
end

% Finds the minimal norm and returns the number that is recognized
[minval, col] = min(min(norms,[],1));
recog = col-1;
end
