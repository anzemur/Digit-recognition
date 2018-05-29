function recog = digitRecognitionSvd(imagePath,data)

image = imread(imagePath);
imageColumn = double(convertToColumn(image));
nSingularVal = 2;
norms = [];
for i=0:9

    s = zeros(size(data(i+1).s));
    s(1:nSingularVal,1:nSingularVal) = data(i+1).s(1:nSingularVal,1:nSingularVal);
    u = data(i+1).u;
    y = (u*s)\imageColumn;
    res = norm(u'*imageColumn - s*y);
    norms = [norms,res];
end
[minval, col] = min(min(norms,[],1));
recog = col-1;
end
