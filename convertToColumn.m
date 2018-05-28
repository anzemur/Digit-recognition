function A =  convertToColumn(image)

numSize = length(image);
A = [];
for j = 1 : numSize
    A = [A;image(:,j)];
end

end
