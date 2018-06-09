function constructImg(img,col)
% constructImg(img,col) - function that takes a vector U from SVD and reconstructs the col column of it back into the image
% img ... U vector from SVD
% col ... reconstruct this column from U
res = [];
img = abs(img .*255)
for i=1:32
    res(:,i) = img((i-1)*32+1:i*32,col);
end

imshow(res);

end
