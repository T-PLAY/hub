

disp("running listStream test");

var = hub_listStream("hello", "my", "name", "is", "gauthier");

disp("----------------------------------------");

var = hub_listStream(666);

disp("----------------------------------------");

var = hub_listStream("127.0.0.1", "bad type of argument");

disp("----------------------------------------");

var = hub_listStream("not a valid ip", 4042);

disp("----------------------------------------");

#hub_listStream
[streamNames, sensorSpecs] = hub_listStream;

#size(streamNames)

#typeinfo(streamNames)

# disp(streamNames);
streamNames
sensorSpecs

# streamNames(1,:)

