
#include <cassert>
#include <filesystem>
#include <iostream>

#include <Player.h>

int main(int argc, char* argv[])
{
    std::string recordFolder;
    int iSensor = -1;
    if (argc == 2) {
//        recordFolder = argv[1];
        iSensor = atoi(argv[1]);
    } else {
//        recordFolder = PROJECT_DIR "data/records/latest/";
    }
        recordFolder = PROJECT_DIR "data/records/latest/";

    std::cout << "record folder : " << recordFolder << std::endl;
    assert(std::filesystem::exists(recordFolder));

    Player player;
        player.load(recordFolder, iSensor);
        player.play();
        std::this_thread::sleep_for(std::chrono::seconds(3600));

//    for (int i = 0; i < 10; ++i) {
//        player.load(recordFolder);
//        player.play();
////        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        player.stop();
//        player.unload();
//    }
    return 0;
}
