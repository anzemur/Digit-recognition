function A = convertToColumn(image)
% convertToColumn(image) - takes an image, stacks it's columns on top of eachother and returns a column vector
% image.. image to convert
% A.. column vector to return

numSize = length(image);
A = [];
for j = 1 : numSize
    A = [A;image(:,j)];
end

end
%!test
%! a = [2 2; 2 2]; expected = [2;2;2;2];
%! assert(convertToColumn(a),expected);
