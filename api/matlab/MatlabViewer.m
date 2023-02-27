


streams = hub_listStreamd;

firstStreamName = streams(1,:);


while (true)

    [X, Y, Z, n, format, start, End] = hub_getAcquisitiond(firstStreamName);


    if format == "6DOF"
        pos = X;
        pos
        quat = Y;
        quat

    elseif format == "RGB8"
        rgb = cat(3, X, Y, Z);
        imshow(rgb);

    elseif format == "BGR8"
        rgb = cat(3, Z, Y, X);
        imshow(rgb);

    elseif format == "Y8"
        imshow(X);

    else
        X
        Y
        Z
    end

    pause(0.01);

end
