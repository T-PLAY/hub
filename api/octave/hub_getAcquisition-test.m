

disp("running getAcquisition test");

streams = hub_listStream;

firstStreamName = streams(1,:)


#var = hub_getAcquisition("hello", "my", "name", "is", "gauthier");

#var = hub_getAcquisition(666);

#var = hub_getAcquisition("127.0.0.1", "bad type of argument");

[X, Y, Z, n, format, start, End] = hub_getAcquisition(firstStreamName);

start
End
n
format
size(X)
size(Y)
size(Z)

#var = hub_getAcquisition;

#size(var)

#typeinfo(var)

# disp(var);


