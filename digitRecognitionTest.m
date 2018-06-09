function digitRecognitionTest(testFunc,folderPath, singVals)
% digitRecognitionTest(testFunc,folderPath, singVals) - function tests either the least squares method or SVD method for how precises it is by sending all the test 
% examples through for the given method 
% testFunc.. takes the function pointer which is to be tested
% folderPath.. Path to the folder containing numbers and test examples

% LeastSquares method: 94%
% Svd : 95%

data = [];

% Load data
for i=0:9
    data = [data,load(['../digit-recognition/svdmatrices/',num2str(i),'.mat'])];
end


correctCnt = 0;
allCnt = 0;
e = 5
% Iterate through test examples for each number and test their accuracy
for i=0:9
    path = strcat(strcat(folderPath,strcat('/',num2str(i))),'/test/');
    dirCnt = length(dir(path))-2;
    for j=70:(70+dirCnt-1)
        imgPath = strcat(path,strcat(num2str(j),'.jpeg'));
        recog = testFunc(imgPath,data,singVals);
        if (recog == i)
            correctCnt++;
        end
        allCnt++;
    end
end

% Print the accuracy
accuracy = (correctCnt/allCnt)*100

end
