%% FUNCTION FILE: calculateError.m

function calculateError(originalImg, decompressedImg)
  O = imread(originalImg);
  D = imread(decompressedImg);
  
  % Red Matrix
  origR = double(O (:,:,1));
  decR = double(D (:,:,1));
  % Green Matrix
  origG = double(O (:,:,2));
  decG = double(D (:,:,2));
  % Blue Matrix
  origB = double(O (:,:,3));
  decB = double(D (:,:,3));

  % Red error  
  errR = norm(origR - decR) / norm(origR);
  % Green error
  errG = norm(origG - decG) / norm(origG);
  % Blue error
  errB = norm(origB - decB) / norm(origB);
  
  err = (errR + errG + errB) / 3;
  
  % Print error
  err
  
endfunction