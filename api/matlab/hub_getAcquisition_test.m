

disp("running getAcquisition test");

streams = hub_listStreamd;

firstStreamName = streams(1,:)


%var = hub_getAcquisitiond("hello", "my", "name", "is", "gauthier");

%var = hub_getAcquisitiond(666);

%var = hub_getAcquisitiond("127.0.0.1", "bad type of argument");

[X, Y, Z, n, format, start, End] = hub_getAcquisitiond(firstStreamName);

start
End
n
format
size(X)
size(Y)
size(Z)

%var = hub_getAcquisitiond;

%size(var)

%typeinfo(var)

% disp(var);


