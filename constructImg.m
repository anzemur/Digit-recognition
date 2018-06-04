function constructImg(img,col)
res = [];
img = abs(img .*255)
for i=1:32
    res(:,i) = img((i-1)*32+1:i*32,col);
end

imshow(res);

end
