


streams = hub_listStream;

firstStreamName = streams(1,:);


while (true)

  [X, Y, Z, n, format, start, End] = hub_getAcquisition(firstStreamName);

#  start
#  End

  if (format == "6DOF")
    pos = X;
    pos
    quat = Y;
    quat

  elseif (format == "RGB8")
#    disp(X);
#    disp(Y);
    rgb = cat(3, X, Y, Z);
    imshow(rgb);

 elseif (format == "BGR8")
    rgb = cat(3, Z, Y, X);
    imshow(rgb);

  elseif (format == "Y8")
    imshow(X);

#  elseif (format == "Y16")
 #   imshow(

  else
     X
     Y
     Z


  endif
  #imshow(img);

  pause(0.01);

  endwhile
