function singValuesTest(testFunc,folderPath,ntest)
% singValuesTest(testFunc,folderPath,ntest) - tests the SVD algorithm accuracy for ntest singular values
% testFunc.. function pointed to the SVD function
% folderPath.. path to the test examples
% ntest.. number of singular values to be tested


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
        % Create the path
        path = strcat(strcat(folderPath,strcat('/',num2str(i))),'/test/');
        dirCnt = length(dir(path))-2;
        % Test
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

% Create a plot
x = linspace(1,ntest,ntest);
plot(x,eff)
title("Tocnost napovedi v odvisnosti od stevila singularnih vrednosti.")

end
