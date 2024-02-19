

#include <iostream>
#include <windows.h>
#include <thread>

int main()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    INPUT_RECORD inp;
    DWORD num_of_events;

    bool exit = false;

    do
    {
//        std::this_thread::sleep_for(std::chrono::milliseconds(20));
//        if (GetKeyState('A') & 0x8000) {
//            std::cout << "A" << std::endl;
//        }

        // Read Console Input
        ReadConsoleInput( hIn, &inp, 1, &num_of_events);

        switch (inp.EventType)
        {
        case KEY_EVENT:
            switch(inp.Event.KeyEvent.wVirtualKeyCode)
            {
            case 0x57:
                std::cout << "W\n";
                break;

            case 0x41:
                std::cout << "A\n";
                break;

            case 0x53:
                std::cout << "S\n";
                break;

            case 0x44:
                std::cout << "D\n";
                break;

            case VK_SPACE:
                std::cout << "Space\n";
                break;

            case VK_F1:
                std::cout << "Help!\n";
                break;

            case VK_ESCAPE:
                exit = true;
                break;

            default:
                std::cout << inp.Event.KeyEvent.wVirtualKeyCode << std::endl;

            }
            break;
        }
    }while(!exit);
}



//#include <iostream>

//#include <core/Utils.hpp>


//int main( int argc, char* argv[] ) {

//        bool exit = false;
//        while ( !exit ) {                             // ESC to quit
//            std::cout << "waiting key ..." << std::endl;
//            const auto key = hub::utils::key_press(); // blocks until a key is pressed
//            std::cout << "input key : " << key << std::endl;

////            switch ( key ) {
////            case hub::utils::Key::F5:
////            case hub::utils::Key::Space:
////                break;
////            case hub::utils::Key::Escape:
////            case hub::utils::Key::Dot:
////                exit = true;
////                break;
////            case hub::utils::Key::h:
////                break;
////            default:
////                std::cout << "unrecognized key : " << key << std::endl;
////            }
//        }
//        std::cout << "exiting" << std::endl;
//}
