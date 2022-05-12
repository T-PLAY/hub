
#include <iostream>
#include <filesystem>
#include <cassert>

#include <Player.h>


int main(int argc, char* argv[])
{
    std::string recordFolder;
    if (argc == 2) {
        recordFolder = argv[1];
    } else {
        recordFolder = PROJECT_DIR "data/records/latest/";
    }

    std::cout << "record folder : " << recordFolder << std::endl;
    assert(std::filesystem::exists(recordFolder));

    Player player;
    player.load(recordFolder);
    player.play();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    player.stop();
    return 0;
}
