
#include <cassert>
#include <chrono>

#include <tuple>

#include <constants.h>

#include <iostream>
#include <stdio.h>
//#include<conio.h>

#include <IO/Stream.hpp>
#include <OutputSensor.hpp>

#ifdef WIN32
#    include <WinUser.h>
#else
//#include <ncurses.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

static auto s_pos                  = glm::vec3( 0.0, 1.0, 0.0 );
static auto s_quat                 = glm::quat( 1.0, 0.0, 0.0, 0.0 ); // w, x, y, z
static bool s_needUpdate           = true;
static constexpr float s_moveSpeed = 1.0;
static bool s_exitApp              = false;

#define margin std::setw( 5 ) << std::setfill( ' ' ) << std::left

void init() {
    s_pos  = glm::vec3( 0.0, 1.0, 0.0 );
    s_quat = glm::quat( 1.0, 0.0, 0.0, 0.0 ); // w, x, y, z
}

void exitApp() {
    s_exitApp = true;
}

void up() {
    //                std::cout << "up" << std::endl;
    s_pos += s_moveSpeed * glm::vec3( 1.0, 0.0, 0.0 );
    //                s_pos += s_moveSpeed * glm::vec3(0.0, 0.0, -1.0);
    //                s_pos += s_moveSpeed * cameraFront;
    s_needUpdate = true;
}

void down() {
    //                std::cout << "down" << std::endl;
    s_pos -= s_moveSpeed * glm::vec3( 1.0, 0.0, 0.0 );
    //                s_pos += s_moveSpeed * glm::vec3(0.0, -1.0, 0.0);
    //                s_pos += s_moveSpeed * glm::vec3(0.0, 0.0, 1.0);
    //                s_pos -= s_moveSpeed * cameraFront;
    s_needUpdate = true;
}

void right() {
    //                std::cout << "right" << std::endl;
    s_pos += s_moveSpeed * glm::vec3( 0.0, 0.0, 1.0 );
    //                s_pos += s_moveSpeed * glm::vec3(1.0, 0.0, 0.0);
    //                s_pos += s_moveSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    s_needUpdate = true;
}

void left() {
    //                std::cout << "left" << std::endl;
    s_pos -= s_moveSpeed * glm::vec3( 0.0, 0.0, 1.0 );
    //                s_pos += s_moveSpeed * glm::vec3(-1.0, 0.0, 0.0);
    //                s_pos -= s_moveSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    s_needUpdate = true;
}

void home() {
    //                std::cout << "page up" << std::endl;
    s_pos += s_moveSpeed * glm::vec3( 0.0, 1.0, 0.0 );
    //                s_pos += s_moveSpeed * glm::vec3(0.0, 1.0, 0.0);
    //                s_pos += s_moveSpeed * cameraUp;
    s_needUpdate = true;
}

void end() {
    //                std::cout << "page down" << std::endl;
    s_pos -= s_moveSpeed * glm::vec3( 0.0, 1.0, 0.0 );
    //                s_pos += s_moveSpeed * glm::vec3(0.0, -1.0, 0.0);
    //                s_pos -= s_moveSpeed * cameraUp;
    s_needUpdate = true;
}

void shiftUp() {
    //                    std::cout << "up" << std::endl;
    //                    pitch -= glm::radians(s_moveSpeed);
    s_quat       = glm::rotate( s_quat, glm::radians( s_moveSpeed ), glm::vec3( 0.0, 0.0, 1.0 ) );
    s_needUpdate = true;
}

void shiftDown() {
    //                    std::cout << "down" << std::endl;
    //                    pitch += glm::radians(s_moveSpeed);
    s_quat       = glm::rotate( s_quat, glm::radians( -s_moveSpeed ), glm::vec3( 0.0, 0.0, 1.0 ) );
    s_needUpdate = true;
}

void shiftRight() {
    //                    std::cout << "right" << std::endl;
    //                    yaw -= glm::radians(s_moveSpeed);
    s_quat       = glm::rotate( s_quat, glm::radians( -s_moveSpeed ), glm::vec3( 0.0, 1.0, 0.0 ) );
    s_needUpdate = true;
}

void shiftLeft() {
    //                    std::cout << "left" << std::endl;
    //                    yaw += glm::radians(s_moveSpeed);
    s_quat = glm::rotate( s_quat, glm::radians( s_moveSpeed ), glm::vec3( 0.0, 1.0, 0.0 ) );
    //                    s_quat = glm::rotate(s_quat, glm::radians(s_moveSpeed), glm::vec3(1.0,
    //                    0.0, 0.0));
    s_needUpdate = true;
}

void shiftHome() {
    //                    std::cout << "page up" << std::endl;
    //                    roll -= glm::radians(s_moveSpeed);
    //                    s_quat = glm::rotate(s_quat, glm::radians(s_moveSpeed),
    //                    glm::vec3(0.0, 1.0, 0.0));
    s_quat       = glm::rotate( s_quat, glm::radians( -s_moveSpeed ), glm::vec3( 1.0, 0.0, 0.0 ) );
    s_needUpdate = true;
}

void shiftEnd() {
    //                    std::cout << "page down" << std::endl;
    //                    roll += glm::radians(s_moveSpeed);
    s_quat       = glm::rotate( s_quat, glm::radians( s_moveSpeed ), glm::vec3( 1.0, 0.0, 0.0 ) );
    s_needUpdate = true;
}

#include <Configurations.hpp>
#include <Streamer.hpp>

int main( int argc, char* argv[] ) {
    //    if ( argc > 2 ) { memcpy( sensorName, argv[1], strlen( argv[1] ) ); }
    int port = hub::net::s_defaultServicePort;
    if ( argc == 2 ) { port = atoi( argv[1] ); }

    char sensorName[80] = "Keyboard";

    std::cout << "sensor name = '" << sensorName << "'" << std::endl;

    //        hub::Header::MetaData metaData;
    hub::SensorSpec::MetaData metaData;
    //    metaData["parent"] = "Keyboard";
    //    metaData["scanWidth"] = 10.0;
    //    metaData["scanDepth"] = 10.0;
    //    metaData["x"] = 0.0;
    //    metaData["y"] = 0.0;
    //    metaData["z"] = 0.0;

    //    hub::OutputSensor proceduralStream("Polhemus Patriot (probe)", Stream::Format::Y8, {
    //    width, height }, hub::ClientSocket(), metaData);

    hub::Streamer streamer( hub::net::s_defaultServiceIp, port );
    streamer.addStream( sensorName,
                        { sensorName, { { { 1 }, hub::SensorSpec::Format::DOF6 } }, metaData } );

    std::thread thread = std::thread( [&]() {
        //        hub::OutputSensor keyboard(
        //            { sensorName, { { { 1 }, hub::SensorSpec::Format::DOF6 } }, metaData },
        //            hub::io::OutputStream( sensorName ) );

        while ( !s_exitApp ) {
            const auto start = std::chrono::high_resolution_clock::now();
            hub::Dof6 dof6( s_pos.x, s_pos.y, s_pos.z, s_quat.w, s_quat.x, s_quat.y, s_quat.z );
            //            assert(translation == glm::value_ptr(s_pos));
            //            assert(orientation == glm::value_ptr(s_quat));
            //            memcpy(data, translation, 12);
            //            memcpy(&data[12], orientation, 16);

#if defined(WIN32)
            const auto maxFps = 70.0;
#else
            const auto maxFps = 60.0;
#endif
            const auto end    = start + std::chrono::microseconds( (int)(1'000'000 / maxFps) );

            const auto& timestampStart =
                std::chrono::duration_cast<std::chrono::microseconds>( start.time_since_epoch() )
                    .count();
            const auto& timestampEnd =
                std::chrono::duration_cast<std::chrono::microseconds>( end.time_since_epoch() )
                    .count();

            //            keyboard << ( hub::Acquisition( timestampStart, timestampEnd ) <<
            //            std::move( dof6 ) );
            streamer.newAcquisition( sensorName,
                                     std::move( hub::Acquisition( timestampStart, timestampEnd )
                                                << std::move( dof6 ) ) );

            std::this_thread::sleep_until( end );
//            std::cout << "+" << std::flush;
        }
    } );

    //    glm::vec3 s_pos(0, -50, -50);
    //    glm::s_quat s_quat(1.0, 0.0, 0.0, 0.0);
    //    auto s_pos = glm::vec3(0, 30, 125);
    //    auto s_pos = glm::vec3(0.0, 1.0, 0.0); // x, y, z
    //    auto s_quat = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
    //    auto s_quat = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
    //    s_quat = glm::rotate(s_quat, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
    //    s_quat = glm::rotate(s_quat, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
    //    s_quat = glm::rotate(s_quat, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
    //    auto s_quat = glm::s_quat(1.0, 0.0, 0.0, 0.0); // w, x, y, z

    //    glm::vec3 cameraFront = glm::vec3(1.0, 0.0, 0.0);
    //    glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
    //    float pitch = 0.0f;
    //    float yaw = glm::radians(90.0f);
    //    float roll = 0.0f;

    //        auto s_quat = glm::s_quat(glm::vec3(pitch, yaw, roll));

    //    unsigned char data[28];
    //    float* translation = glm::value_ptr(s_pos); // x, y, z
    //    float* orientation = glm::value_ptr(s_quat); // x, y, z, w
    //    memcpy(data, translation, 12);
    //    memcpy(&data[12], orientation, 16);
    //    for (int i = 0; i < 7; ++i) {
    //        std::cout << ((float*)data)[i] << ", ";
    //    }
    //    std::cout << std::endl;

    //    KEY_SLEFT;
    char c;
#ifdef WIN32
    //    HWND mainWindow = GetConsoleWindow();
    HWND mainWindow = GetForegroundWindow(); // assume here, foregroundWindow is the window of this
                                             // console application
    std::cout << "[main] mainWindow = " << mainWindow << std::endl;
#else
    // Set the terminal to raw mode
    system( "stty raw" );
#endif
    while ( !s_exitApp ) {

#ifdef WIN32
        //        HWND activeWindow = GetActiveWindow();
        //        HWND activeWindow = GetCurrentProcess();
        //        HWND activeWindow = GetFocus();
        HWND activeWindow = GetForegroundWindow();
        //        std::cout << "[main] activeWindow = " << activeWindow << std::endl;
        if ( activeWindow != mainWindow ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            continue;
        }
        bool shifted = false;
        if ( GetKeyState( VK_SHIFT ) & 0x8000 ) {
            //            std::cout << "shift down " << std::endl;
            // shift down
            shifted = true;
        }
        //        if ( GetKeyState( VK_ESCAPE ) & 0x8000 ) {
        if ( GetKeyState( VK_OEM_PERIOD ) & 0x8000 || GetKeyState( VK_ESCAPE ) & 0x8000 ) {
            std::cout << std::endl;
            exitApp();
            //            exit( 0 );
        }
        if ( GetKeyState( VK_RIGHT ) & 0x8000 ) {
            if ( shifted ) { shiftRight(); }
            else { right(); }
        }
        if ( GetKeyState( VK_LEFT ) & 0x8000 ) {
            if ( shifted ) { shiftLeft(); }
            else { left(); }
        }
        if ( GetKeyState( VK_UP ) & 0x8000 ) {
            if ( shifted ) { shiftUp(); }
            else { up(); }
        }
        if ( GetKeyState( VK_DOWN ) & 0x8000 ) {
            if ( shifted ) { shiftDown(); }
            else { down(); }
        }
        if ( GetKeyState( VK_HOME ) & 0x8000 ) {
            if ( shifted ) { shiftHome(); }
            else { home(); }
        }
        if ( GetKeyState( VK_END ) & 0x8000 ) {
            if ( shifted ) { shiftEnd(); }
            else { end(); }
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );

#else
        std::string inputKeyStr;
        //        std::string getString(char c) {
        //            return std::to_string(c);
        //        }
        auto getString = []( char c ) { return std::to_string( c ) + " "; };
        c              = getchar();
        inputKeyStr += getString( c );
        // terminate when "." is pressed
        if ( c == '\033' ) {
            c = getchar();
            inputKeyStr += getString( c );
            c = getchar();
            inputKeyStr += getString( c );
            switch ( c ) {
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
                c = getchar();
                inputKeyStr += getString( c );
                if ( c == 59 ) {

                    c = getchar();
                    inputKeyStr += getString( c );
                    c = getchar();
                    inputKeyStr += getString( c );
                    //                std::cout << "shift ";
                    switch ( c ) {
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
                }
                // F5
                else if ( c == 53 ) {
                    //                    std::cout << "F5" << std::endl;
                    init();
                }
            } // switch (getchar()) 1
        }
        else if ( c == '.' ) {
            system( "stty cooked" );
            exitApp();
            //            exit( 0 );
        }
//        std::cout << "input key : '" << inputKeyStr << "'" << std::endl;
#endif

        if ( s_needUpdate ) {
            //            std::cout << "update stream" << std::endl;
            s_needUpdate = false;
            //            s_quat = glm::s_quat(glm::vec3(pitch, yaw, roll));
            //            glm::vec3 front;
            //            front.x = cos(yaw) * cos(pitch);
            //            front.y = sin(pitch);
            //            front.z = sin(yaw) * cos(pitch);
            //            cameraFront = glm::normalize(front);

            //            std::cout << "\r"
            if ( streamer.isConnected() ) {
                std::cout
#ifdef OS_LINUX
                    << "\r"
#endif
                    << "\tx:" << margin << s_pos.x << " y:" << margin << s_pos.y << " z:" << margin
                    << s_pos.z << "\t\t"
                    << " w:" << margin << s_quat.w << " x:" << margin << s_quat.x << " y:" << margin
                    << s_quat.y << " z:" << margin << s_quat.z
#ifdef OS_WINDOWS
                    << std::endl
#endif
                    ;
            }
            //                << std::flush;
        } // if (s_needUpdate)
        //        std::cout << c << " was pressed."<< std::endl;
        //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    } // while (1)

    thread.join();

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
//            mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", c,
//            c); refresh(); break;
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
////                mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'",
/// c, c); /                refresh(); /                break; /        } / print_menu(menu_win,
/// highlight); /        if(choice != 0)	/* User did a choice come out of the infinite loop */ /
/// break; /    } /    mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice,
/// choices[choice - 1]); /    clrtoeol(); /    refresh(); /    endwin(); /    return 0;
////}
