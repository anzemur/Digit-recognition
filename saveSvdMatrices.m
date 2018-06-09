function saveSvdMatrices(saveFolder)
% saveSvdMatrices(saveFolder) - creatings learning matrix A, makes the SVD decomposition and saves A,U and S for each number into the saveFolder
% saveFolder.. folder path where we save all the .mat files for each number

for i=0:9
    a = buildLearningMatrix('../digit-recognition/digits/',i);
    % Normalize to 0..1
    a = a ./ 255;
    [u,s,v] = svd(a);
    save([saveFolder,num2str(i),'.mat'],'u','s','a');
end

end
