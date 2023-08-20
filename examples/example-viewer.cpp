

//#include <
#include <client/Viewer.hpp>
#include <client/ViewerInterface.hpp>

int main(int argc, char* argv[])
{

    hub::client::ViewerMqtt viewer("127.0.0.1", 1883);

    std::cout << "press key to quit" << std::endl;
    getchar();

    return 0;
}
