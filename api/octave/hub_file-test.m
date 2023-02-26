

disp("running hub test");

img = hub_file();

size(img)

typeinfo(img)


imwrite(img, "output.png");

