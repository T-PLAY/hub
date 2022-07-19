
#include <cassert>
#include <chrono>

#include <socket.h>
#include <stream.h>
#include <tuple>

#include <constants.h>

#include <iostream>
#include <stdio.h>
//#include<conio.h>

#ifdef WIN32
#include <WinUser.h>
#else
//#include <ncurses.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

auto pos = glm::vec3(0.0, 1.0, 0.0);
auto quat = glm::quat(1.0, 0.0, 0.0, 0.0); // w, x, y, z
bool needUpdate = true;
const float moveSpeed = 5.0;

void up()
{
    //                std::cout << "up" << std::endl;
    pos += moveSpeed * glm::vec3(1.0, 0.0, 0.0);
    //                pos += moveSpeed * glm::vec3(0.0, 0.0, -1.0);
    //                pos += moveSpeed * cameraFront;
    needUpdate = true;
}

void down()
{
    //                std::cout << "down" << std::endl;
    pos -= moveSpeed * glm::vec3(1.0, 0.0, 0.0);
    //                pos += moveSpeed * glm::vec3(0.0, -1.0, 0.0);
    //                pos += moveSpeed * glm::vec3(0.0, 0.0, 1.0);
    //                pos -= moveSpeed * cameraFront;
    needUpdate = true;
}

void right()
{
    //                std::cout << "right" << std::endl;
    pos += moveSpeed * glm::vec3(0.0, 0.0, 1.0);
    //                pos += moveSpeed * glm::vec3(1.0, 0.0, 0.0);
    //                pos += moveSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    needUpdate = true;
}

void left()
{
    //                std::cout << "left" << std::endl;
    pos -= moveSpeed * glm::vec3(0.0, 0.0, 1.0);
    //                pos += moveSpeed * glm::vec3(-1.0, 0.0, 0.0);
    //                pos -= moveSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    needUpdate = true;
}

void home()
{
    //                std::cout << "page up" << std::endl;
    pos += moveSpeed * glm::vec3(0.0, 1.0, 0.0);
    //                pos += moveSpeed * glm::vec3(0.0, 1.0, 0.0);
    //                pos += moveSpeed * cameraUp;
    needUpdate = true;
}

void end()
{
    //                std::cout << "page down" << std::endl;
    pos -= moveSpeed * glm::vec3(0.0, 1.0, 0.0);
    //                pos += moveSpeed * glm::vec3(0.0, -1.0, 0.0);
    //                pos -= moveSpeed * cameraUp;
    needUpdate = true;
}

void shiftUp()
{
    //                    std::cout << "up" << std::endl;
    //                    pitch -= glm::radians(moveSpeed);
    quat = glm::rotate(quat, glm::radians(moveSpeed), glm::vec3(0.0, 0.0, 1.0));
    needUpdate = true;
}

void shiftDown()
{
    //                    std::cout << "down" << std::endl;
    //                    pitch += glm::radians(moveSpeed);
    quat = glm::rotate(quat, glm::radians(-moveSpeed), glm::vec3(0.0, 0.0, 1.0));
    needUpdate = true;
}

void shiftRight()
{
    //                    std::cout << "right" << std::endl;
    //                    yaw -= glm::radians(moveSpeed);
    quat = glm::rotate(quat, glm::radians(-moveSpeed), glm::vec3(0.0, 1.0, 0.0));
    needUpdate = true;
}

void shiftLeft()
{
    //                    std::cout << "left" << std::endl;
    //                    yaw += glm::radians(moveSpeed);
    quat = glm::rotate(quat, glm::radians(moveSpeed), glm::vec3(0.0, 1.0, 0.0));
    //                    quat = glm::rotate(quat, glm::radians(moveSpeed), glm::vec3(1.0, 0.0, 0.0));
    needUpdate = true;
}

void shiftHome()
{
    //                    std::cout << "page up" << std::endl;
    //                    roll -= glm::radians(moveSpeed);
    //                    quat = glm::rotate(quat, glm::radians(moveSpeed), glm::vec3(0.0, 1.0, 0.0));
    quat = glm::rotate(quat, glm::radians(-moveSpeed), glm::vec3(1.0, 0.0, 0.0));
    needUpdate = true;
}

void shiftEnd()
{
    //                    std::cout << "page down" << std::endl;
    //                    roll += glm::radians(moveSpeed);
    quat = glm::rotate(quat, glm::radians(moveSpeed), glm::vec3(1.0, 0.0, 0.0));
    needUpdate = true;
}

int main(int argc, char* argv[])
{

    Stream::MetaData metaData;
    //    metaData["scanWidth"] = 10.0;
    //    metaData["scanDepth"] = 10.0;
    //    metaData["x"] = 0.0;
    //    metaData["y"] = 0.0;
    //    metaData["z"] = 0.0;

    //    OutputStream proceduralStream("Polhemus Patriot (probe)", Stream::Format::Y8, { width, height }, ClientSocket(), metaData);
    OutputStream posStream(g_probePoseSensorName, Stream::Format::DOF6, { 1 }, ClientSocket(), metaData);

    //    glm::vec3 pos(0, -50, -50);
    //    glm::quat quat(1.0, 0.0, 0.0, 0.0);
    //    auto pos = glm::vec3(0, 30, 125);
    //    auto pos = glm::vec3(0.0, 1.0, 0.0); // x, y, z
    //    auto quat = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
    //    auto quat = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
    //    quat = glm::rotate(quat, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
    //    quat = glm::rotate(quat, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
    //    quat = glm::rotate(quat, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
    //    auto quat = glm::quat(1.0, 0.0, 0.0, 0.0); // w, x, y, z

    //    glm::vec3 cameraFront = glm::vec3(1.0, 0.0, 0.0);
    //    glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
    //    float pitch = 0.0f;
    //    float yaw = glm::radians(90.0f);
    //    float roll = 0.0f;

    //        auto quat = glm::quat(glm::vec3(pitch, yaw, roll));

    unsigned char data[28];

    float* translation = glm::value_ptr(pos); // x, y, z
    float* orientation = glm::value_ptr(quat); // x, y, z, w
    memcpy(data, translation, 12);
    memcpy(&data[12], orientation, 16);
    for (int i = 0; i < 7; ++i) {
        std::cout << ((float*)data)[i] << ", ";
    }
    std::cout << std::endl;

    //    KEY_SLEFT;
    char c;
#ifndef WIN32
    // Set the terminal to raw mode
    system("stty raw");
#endif
    while (1) {

#ifdef WIN32
        bool shifted = false;
        if (GetKeyState(VK_SHIFT) & 0x8000) {
            //            std::cout << "shift down " << std::endl;
            // shift down
            shifted = true;
        }
        if (GetKeyState(VK_ESCAPE) & 0x8000) {
            std::cout << std::endl;
            exit(0);
        }
        if (GetKeyState(VK_RIGHT) & 0x8000) {
            if (shifted) {
                shiftRight();
            } else {
                right();
            }
        }
        if (GetKeyState(VK_LEFT) & 0x8000) {
            if (shifted) {
                shiftLeft();
            } else {
                left();
            }
        }
        if (GetKeyState(VK_UP) & 0x8000) {
            if (shifted) {
                shiftUp();
            } else {
                up();
            }
        }
        if (GetKeyState(VK_DOWN) & 0x8000) {
            if (shifted) {
                shiftDown();
            } else {
                down();
            }
        }
        if (GetKeyState(VK_HOME) & 0x8000) {
            if (shifted) {
                shiftHome();
            } else {
                home();
            }
        }
        if (GetKeyState(VK_END) & 0x8000) {
            if (shifted) {
                shiftEnd();
            } else {
                end();
            }
        }

#else
        c = getchar();
        // terminate when "." is pressed
        if (c == '\033') {
            getchar();
            switch (getchar()) {
            case 'A': // up
                up();
                break;
            case 'B': // down
                down();
                break;

            case 'C': // right
                right();
                break;
            case 'D': // left
                left();
                break;

            case 'H': // page up
                home();
                break;
            case 'F': // page down
                end();
                break;

            case '1':
                getchar();
                getchar();
                //                std::cout << "shift ";
                switch (getchar()) {
                case 'A': // up
                    shiftUp();
                    break;
                case 'B': // down
                    shiftDown();
                    break;

                case 'C': // right
                    shiftRight();
                    break;
                case 'D': // left
                    shiftLeft();
                    break;

                case 'H': // page up
                    shiftHome();
                    break;
                case 'F': // page down
                    shiftEnd();
                    break;
                    //                assert(-90 < pitch && pitch < 90);
                } // switch getchar() 2
                break;
            } // switch (getchar()) 1
        } else if (c == '.') {
            system("stty cooked");
            exit(0);
        }
#endif

        if (needUpdate) {
            //            std::cout << "update stream" << std::endl;
            needUpdate = false;
            //            quat = glm::quat(glm::vec3(pitch, yaw, roll));
            //            glm::vec3 front;
            //            front.x = cos(yaw) * cos(pitch);
            //            front.y = sin(pitch);
            //            front.z = sin(yaw) * cos(pitch);
            //            cameraFront = glm::normalize(front);

            assert(translation == glm::value_ptr(pos));
            assert(orientation == glm::value_ptr(quat));
            memcpy(data, translation, 12);
            memcpy(&data[12], orientation, 16);
            const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            posStream << Stream::Acquisition(timestampStart, timestampStart, data, 28);

            std::cout << "\r"
                      << "x:" << pos.x << " y:" << pos.y << " z:" << pos.z << "\t"
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
