function singValuesTest(testFunc,folderPath,ntest)



% LeastSquares method: 94%
data = [];
% Load data
for i=0:9
    data = [data,load(['../digit-recognition/svdmatrices/',num2str(i),'.mat'])];
end
eff = [];
for k=1:ntest
    correctCnt = 0;
    allCnt = 0;
    for i=0:9
        path = strcat(strcat(folderPath,strcat('/',num2str(i))),'/test/');
        dirCnt = length(dir(path))-2;
        for j=70:(70+dirCnt-1)
            imgPath = strcat(path,strcat(num2str(j),'.jpeg'));
            recog = testFunc(imgPath,data,k);
            if (recog == i)
                correctCnt++;
            end
            allCnt++;
        end
    end

    res = (correctCnt/allCnt)*100;
    eff = [eff,res];
end
x = linspace(1,ntest,ntest);
plot(x,eff);

end
