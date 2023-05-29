1;
clc;
clear;

%% PARTE 1

tam = 256;
h = 3;

I = zeros(tam, tam, 3);

for x = 1:tam
  for y = 1:tam
    I(x,y,1) = sin(h*x);
    I(x,y,2) = (sin(h*y)+sin(h*x))/2;
    I(x,y,3) = sin(h*x);
  endfor
endfor

I = im2double(I);
imshow(I)
imwrite (I, 'generatedImg.png', 'png');

compress ('generatedImg.png', 2);
decompress ('compressedImg.png', 2, h);
