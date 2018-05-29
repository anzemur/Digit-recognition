function saveSvdMatrices(saveFolder)


for i=0:9
    a = buildLearningMatrix('../digit-recognition/digits/',i);
    [u,s,v] = svd(a);
    save([saveFolder,num2str(i),'.mat'],'u','s','a');
end

end
