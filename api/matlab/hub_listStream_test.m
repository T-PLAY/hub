




disp("running listStream test");

var = hub_listStreamd("hello", "my", "name", "is", "gauthier")

disp("----------------------------------------");

var = hub_listStreamd(666);

disp("----------------------------------------");

var = hub_listStreamd("127.0.0.1", "bad type of argument");

disp("----------------------------------------");

var = hub_listStreamd("not a valid ip", 4042);

disp("----------------------------------------");

% hub_listStream
[streamNames, sensorSpecs] = hub_listStreamd;

size(streamNames)

class(streamNames)

% disp(streamNames);
streamNames
sensorSpecs

% streamNames(1,:)

