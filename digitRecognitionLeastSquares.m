function digitRecognitionLeastSquares(imagePath)

image = imread(imagePath);
imageColumn = double(convertToColumn(image));

norms = [];
for i=0 : 9
    a = double(buildLearningMatrix('../digit-recognition/digits/',i));
    x = a\imageColumn;
    norms = [norms,norm(imageColumn - a*x)];
end
[minval, col] = min(min(norms,[],1));
norms
res = col-1
end
