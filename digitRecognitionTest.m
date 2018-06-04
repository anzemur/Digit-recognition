function digitRecognitionTest(testFunc,folderPath, singVals)



% LeastSquares method: 94%
data = [];
% Load data
for i=0:9
    data = [data,load(['../digit-recognition/svdmatrices/',num2str(i),'.mat'])];
end


correctCnt = 0;
allCnt = 0;
e = 5
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

(correctCnt/allCnt)*100

end
