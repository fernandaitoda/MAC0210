%% FUNCTION FILE: DECOMPRESS.m

%% CÁLCULO DE DERIVADAS
% Para pontos interiores: Método da Diferença Centralizada
% Para pontos da borda: usar diferença unilateral correspondente

function decompress(compressedImg, k, h)
  I = imread(compressedImg); 
  I = im2double (I);
  n = rows(I);

  % Red Matrix
  R = I(:,:,1);
  % Green Matrix
  G = I(:, :, 2);
  % Blue Matrix
  B = I(:, :, 3);
  
  bcR = Bicubic(R, h, k);
  bcG = Bicubic(G, h, k);
  bcB = Bicubic(B, h, k);
 
  new = cat(3, bcR, bcG, bcB);
  
  blR = Bilinear(R, h, k);
  blG = Bilinear(G, h, k);
  blB = Bilinear(B, h, k);
  
  new2 = cat(3, blR, blG, blB);
 
  imwrite (new, 'decompressedImg1.png', 'png');
  imwrite (new2, 'decompressedImg2.png', 'png');
  
endfunction

function Q = Bicubic (M, h, k)
% Sistema do tipo Mat^(-1)*A*Mat'^(-1) = f

  Mat = [ 1  0   0     0
          1  h  h.^2  h.^3
          0  1   0     0
          0  1  2*h   3*h.^2 ]; 
  
  % Coloca k zeros entre cada elemento da matriz M
  out = zeros (columns(M) + (k*(columns(M)-1)));   
  out (1:k+1:end,1:k+1:end) = M(:,:);  
  
  % Calcula o quadrado new e substitui na matriz out o quadrado calculado
  indOutI = 1;
  for i = 1:rows(M)-1
    indOutJ = 1;
    for j = 1:columns(M)-1
      new = getSquare(i,j);
      out (indOutI:indOutI+k, indOutJ:indOutJ+k) = new(:,:);
      indOutJ += k+1;
    endfor
    indOutI += k+1;
  endfor
  
  function sqr = getSquare (i, j)
    coef = getCoef(i,j);
    
    xi = M(i,j);
    yj = M(i,j);
    
    p = 0:k;
    
    xp = xi + p * (h/(k+1));
    yp = yj + p * (h/(k+1));

    sqr = [];
    for f = 1:k+1
      line = [];
      for ff = 1:k+1
        line = [line P(xi, yj, coef, xp(f), yp(ff))];
      endfor
      sqr = [sqr; line];
    endfor 
  endfunction
  
  function px = P(xi, yj, coef, x,y)
    px = [1 (x-xi) (x-xi).^2 (x-xi).^3];
    px = double(px) * coef;
    px = px * double ([1; (y-yj); (y-yj).^2; (y-yj).^3]);
  endfunction
      
  function coef = getCoef (i, j)
    f = [ M(i,j)     M(i,j+1)     Dy(i,j)     Dy(i,j+1)
          M(i+1,j)   M(i+1,j+1)   Dy(i+1,j)   Dy(i+1,j+1)
          Dx(i,j)    Dx(i,j+1)    Dxy(i,j)    Dxy(i,j+1)
          Dx(i+1,j)  Dx(i+1,j+1)  Dxy(i+1,j)  Dxy(i+1,j+1) ];

    f = double (f);
    coef = inv(Mat) * f *  inv(Mat');
  endfunction  
  
  function d = Dx (i, j)    
    if (i != 1 && i != rows(M))
      d = (M(i+1,j) - M(i-1,j)) / (2*h);
    elseif (i != 1)
      d = (M(i,j) - M(i-1,j)) / (2*h);
    else
      d = (M(i+1,j) - M(i,j)) / (2*h);
    endif
  endfunction

  function d = Dy (i, j)
    global t;
    
    if (j != 1 && j != columns(M))
      d = (M(i,j+1) - M(i,j-1)) / (2*h);
    elseif (j != 1)
      d = (M(i,j) - M(i,j-1)) / (2*h);
    else
      d = (M(i,j+1) - M(i,j)) / (2*h);
    endif
  endfunction

  function d = Dxy (i, j)
    if (i != 1 && i != rows(M))
      d = (Dy(i+1,j) - Dy(i-1,j))/(2*h);
    elseif (i != 1)
      d = (Dy(i,j) - Dy(i-1,j))/(2*h);
    else
      d = (Dy(i+1,j) - Dy(i,j))/(2*h);
    endif
  endfunction

  Q = out;  
endfunction

function Q = Bilinear (M, h, k)
% Sistema do tipo Ax = b
  
  A = [ 1 0 0  0
        1 0 h  0
        1 h 0  0
        1 h h h^2 ];
  
  % Coloca k zeros entre cada elemento da matriz M
  out = zeros (columns(M) + (k*(columns(M)-1)));   
  out (1:k+1:end,1:k+1:end) = M(:,:);  
  
  % Calcula o quadrado new e substitui na matriz out o quadrado calculado
  indOutI = 1;
  for i = 1:rows(M)-1
    indOutJ = 1;
    for j = 1:columns(M)-1
      new = getSquare(i,j);
      out (indOutI:indOutI+k, indOutJ:indOutJ+k) = new(:,:);
      indOutJ += k+1;
    endfor
    indOutI += k+1;
  endfor
  
  function sqr = getSquare (i, j)
    coef = getCoef(i,j);
    xi = M(i,j);
    yj = M(i,j);
    
    p = 0:k;
    
    xp = xi + p * (h/(k+1));
    yp = yj + p * (h/(k+1));

    sqr = [];
    for f = 1:k+1
      line = [];
      for ff = 1:k+1
        line = [line P(xi, yj, coef, xp(f), yp(ff))];
      endfor
      sqr = [sqr; line];
    endfor 
  endfunction
  
  function px = P(xi, yj, coef, xp,yp)
    px = coef(1)+coef(2)*(xp-xi)+coef(3)*(yp-yj)+coef(4)*(xp-xi)*(yp-yj);
  endfunction
      
  function coef = getCoef (i, j)
    b = [ M(i,j)
          M(i,j+1) 
          M(i+1,j) 
          M(i+1,j+1) ];
    
    b = double (b);
    coef = A\b;
  endfunction  
  
  Q = out;
endfunction
