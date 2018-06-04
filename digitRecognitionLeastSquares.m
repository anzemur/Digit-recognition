function recog = digitRecognitionLeastSquares(imagePath,data,singVals)

image = imread(imagePath);
imageColumn = double(convertToColumn(image));

norms = [];
for i=0 : 9
    a = data(i+1).a;
    x = a\imageColumn;
    norms = [norms,norm(imageColumn - a*x)];
end
[minval, col] = min(min(norms,[],1));
recog = col-1;
end
