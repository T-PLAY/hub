


streams = hub_listStream;

firstStreamName = streams(1,:);


while (true)

  [start, End, n, X, Y, Z] = hub_getAcquisition(firstStreamName);

  if (n == 1)
    # disp(X);
    imshow(X);

  elseif (n == 2)
    disp(X);
    disp(Y);

  else
    rgb = cat(3, Z, Y, X);
    imshow(rgb);

  endif
  #imshow(img);

  pause(0.01);

  endwhile
