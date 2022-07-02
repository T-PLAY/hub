
#include <cassert>
#include <chrono>

#include <socket.h>
#include <stream.h>
#include <tuple>

#include <constants.h>

#include <iostream>
#include <stdio.h>
//#include<conio.h>
#include <ncurses.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char* argv[])
{

    Stream::MetaData metaData;
    metaData["scanWidth"] = 10.0;
    metaData["scanDepth"] = 10.0;
    metaData["x"] = 0.0;
    metaData["y"] = 0.0;
    metaData["z"] = 0.0;


    //    OutputStream proceduralStream("Polhemus Patriot (probe)", Stream::Format::Y8, { width, height }, ClientSocket(), metaData);
    OutputStream posStream(g_probePoseSensorName, Stream::Format::DOF6, { 1 }, ClientSocket(), metaData);

    glm::vec3 pos(0, -50, -50);
    glm::quat quat(1.0, 0.0, 0.0, 0.0);
    //    quat = glm::rotate(quat, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
    //    quat = glm::rotate(quat, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
    //    quat = glm::rotate(quat, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));

    unsigned char data[28];

    float* translation = glm::value_ptr(pos);
    float* orientation = glm::value_ptr(quat);
    memcpy(data, translation, 12);
    memcpy(&data[12], orientation, 16);

    //    KEY_SLEFT;
    char c;
    bool needUpdate = true;
    // Set the terminal to raw mode
    system("stty raw");
    while (1) {
        c = getchar();
        // terminate when "." is pressed
        if (c == '\033') {
            getchar();
            switch (getchar()) {
            case 'A': // up
                //                std::cout << "up" << std::endl;
                pos += glm::vec3(0.0, 1.0, 0.0);
                needUpdate = true;
                break;
            case 'B': // down
                //                std::cout << "down" << std::endl;
                pos += glm::vec3(0.0, -1.0, 0.0);
                needUpdate = true;
                break;

            case 'C': // right
                //                std::cout << "right" << std::endl;
                pos += glm::vec3(-1.0, 0.0, 0.0);
                needUpdate = true;
                break;
            case 'D': // left
                //                std::cout << "left" << std::endl;
                pos += glm::vec3(1.0, 0.0, 0.0);
                needUpdate = true;
                break;

            case 'H': // page up
                //                std::cout << "page up" << std::endl;
                pos += glm::vec3(0.0, 0.0, -1.0);
                needUpdate = true;
                break;
            case 'F': // page down
                //                std::cout << "page down" << std::endl;
                pos += glm::vec3(0.0, 0.0, 1.0);
                needUpdate = true;
                break;

            case '1':
                getchar();
                getchar();
                //                std::cout << "shift ";
                switch (getchar()) {
                case 'A': // up
                    //                    std::cout << "up" << std::endl;
                    quat = glm::rotate(quat, glm::radians(1.0f), glm::vec3(0.0, 0.0, 1.0));
                    needUpdate = true;
                    break;
                case 'B': // down
                    //                    std::cout << "down" << std::endl;
                    quat = glm::rotate(quat, glm::radians(1.0f), glm::vec3(0.0, 0.0, -1.0));
                    needUpdate = true;
                    break;

                case 'C': // right
                    //                    std::cout << "right" << std::endl;
                    quat = glm::rotate(quat, glm::radians(1.0f), glm::vec3(-1.0, 0.0, 0.0));
                    needUpdate = true;
                    break;
                case 'D': // left
                    //                    std::cout << "left" << std::endl;
                    quat = glm::rotate(quat, glm::radians(1.0f), glm::vec3(1.0, 0.0, 0.0));
                    needUpdate = true;
                    break;

                case 'F': // page down
                    //                    std::cout << "page down" << std::endl;
                    quat = glm::rotate(quat, glm::radians(1.0f), glm::vec3(0.0, -1.0, 0.0));
                    needUpdate = true;
                    break;
                case 'H': // page up
                    //                    std::cout << "page up" << std::endl;
                    quat = glm::rotate(quat, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
                    needUpdate = true;
                    break;
                }
                break;
            }

        } else if (c == '.') {
            system("stty cooked");
            exit(0);
        }

        if (needUpdate) {
            //            std::cout << "update stream" << std::endl;
            needUpdate = false;
            assert(translation == glm::value_ptr(pos));
            assert(orientation == glm::value_ptr(quat));
            memcpy(data, translation, 12);
            memcpy(&data[12], orientation, 16);
            const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            posStream << Stream::Acquisition(timestampStart, timestampStart, data, 28);

            std::cout << "\r"
                      << "x:" << pos.x << " y:" << pos.y << " z:" << pos.z
                      << " w:" << quat.w << " x:" << quat.x << " y:" << quat.y << " z:" << quat.z << "                  ";
        }
        //        std::cout << c << " was pressed."<< std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    //    char c;
    //    while(1){ // infinite loop
    //        c = getch();
    //        cout << c;
    //    }

    return 0;
}

//#include <ncurses.h>
////#include <stdio.h>
//#include <iostream>

// int main()
//{
//     initscr(); /* Start curses mode 		  */
//     printw("Hello World !!!"); /* Print Hello World		  */
//     refresh(); /* Print it on to the real screen */
//     //    getch();			/* Wait for user input */
//     while (1) {
//         int c = getch();
//         switch (c) {
////        case 27:
////            exit(0);
////            break;
//        case KEY_UP:
//            std::cout << "up" << std::endl;
//            break;
//        case KEY_DOWN:
//            break;
//        case KEY_LEFT:
//            break;
//        case KEY_RIGHT:
//            break;
////        case KEY_SUP:
////            break;
////        case KEY_SDOWN:
////            break;
//        case KEY_PAGEUP:
//            break;
//        case KEY_SLEFT:
//            break;
////        case 10:
////            break;
//        default:
//            mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
//            refresh();
//            break;
//        }
//    }
//    endwin(); /* End curses mode		  */

//    return 0;
//}

////#define WIDTH 30
////#define HEIGHT 10

//// int startx = 0;
//// int starty = 0;

//// char *choices[] = {
////             "Choice 1",
////             "Choice 2",
////             "Choice 3",
////             "Choice 4",
////             "Exit",
////           };
//// int n_choices = sizeof(choices) / sizeof(char *);
//// void print_menu(WINDOW *menu_win, int highlight);

//// int main()
////{
////     WINDOW *menu_win;
////     int highlight = 1;
////     int choice = 0;
////     int c;

////    initscr();
////    clear();
////    noecho();
////    cbreak();	/* Line buffering disabled. pass on everything */
////    startx = (80 - WIDTH) / 2;
////    starty = (24 - HEIGHT) / 2;

////    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
////    keypad(menu_win, TRUE);
////    mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
////    refresh();
////    print_menu(menu_win, highlight);
////    while(1)
////    {	c = wgetch(menu_win);
////        switch(c)
////        {	case KEY_UP:
////                if(highlight == 1)
////                    highlight = n_choices;
////                else
////                    --highlight;
////                break;
////            case KEY_DOWN:
////                if(highlight == n_choices)
////                    highlight = 1;
////                else
////                    ++highlight;
////                break;
////            case 10:
////                choice = highlight;
////                break;
////            default:
////                mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
////                refresh();
////                break;
////        }
////        print_menu(menu_win, highlight);
////        if(choice != 0)	/* User did a choice come out of the infinite loop */
////            break;
////    }
////    mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
////    clrtoeol();
////    refresh();
////    endwin();
////    return 0;
////}
