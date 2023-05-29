%% FUNCTION FILE: COMPRESS.m
% comprime a imagem 'file' de pxp para nxn tal que p = n+(n-1)*k

function compress (originalImg, k)
  I = imread(originalImg); 
  p = rows(I);
  
  i = find(mod(1:p, k+1)==1);

  % Gera nova imagem comprimida
  Icompressed = I(i, i, :);
  imwrite (Icompressed, 'compressedImg.png', 'png');

endfunction
