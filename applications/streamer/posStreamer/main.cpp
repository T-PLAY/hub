
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
    s_pos += s_moveSpeed * glm::vec3( 1.0, 0.0, 0.0 );
    s_needUpdate = true;
}

void down() {
    s_pos -= s_moveSpeed * glm::vec3( 1.0, 0.0, 0.0 );
    s_needUpdate = true;
}

void right() {
    s_pos += s_moveSpeed * glm::vec3( 0.0, 0.0, 1.0 );
    s_needUpdate = true;
}

void left() {
    s_pos -= s_moveSpeed * glm::vec3( 0.0, 0.0, 1.0 );
    s_needUpdate = true;
}

void home() {
    s_pos += s_moveSpeed * glm::vec3( 0.0, 1.0, 0.0 );
    s_needUpdate = true;
}

void end() {
    s_pos -= s_moveSpeed * glm::vec3( 0.0, 1.0, 0.0 );
    s_needUpdate = true;
}

void shiftUp() {
    s_quat       = glm::rotate( s_quat, glm::radians( s_moveSpeed ), glm::vec3( 0.0, 0.0, 1.0 ) );
    s_needUpdate = true;
}

void shiftDown() {
    s_quat       = glm::rotate( s_quat, glm::radians( -s_moveSpeed ), glm::vec3( 0.0, 0.0, 1.0 ) );
    s_needUpdate = true;
}

void shiftRight() {
    s_quat       = glm::rotate( s_quat, glm::radians( -s_moveSpeed ), glm::vec3( 0.0, 1.0, 0.0 ) );
    s_needUpdate = true;
}

void shiftLeft() {
    s_quat       = glm::rotate( s_quat, glm::radians( s_moveSpeed ), glm::vec3( 0.0, 1.0, 0.0 ) );
    s_needUpdate = true;
}

void shiftHome() {
    s_quat       = glm::rotate( s_quat, glm::radians( -s_moveSpeed ), glm::vec3( 1.0, 0.0, 0.0 ) );
    s_needUpdate = true;
}

void shiftEnd() {
    s_quat       = glm::rotate( s_quat, glm::radians( s_moveSpeed ), glm::vec3( 1.0, 0.0, 0.0 ) );
    s_needUpdate = true;
}

#include <Configurations.hpp>
#include <Streamer.hpp>

int main( int argc, char* argv[] ) {
    int port = hub::net::s_defaultServicePort;
    if ( argc == 2 ) { port = atoi( argv[1] ); }

    char sensorName[80] = "Keyboard";

    std::cout << "sensor name = '" << sensorName << "'" << std::endl;

    hub::SensorSpec::MetaData metaData;

    hub::Streamer streamer( hub::net::s_defaultServiceIp, port );
    streamer.addStream( sensorName,
                        { sensorName, { { { 1 }, hub::SensorSpec::Format::DOF6 } }, metaData } );

    std::thread thread = std::thread( [&]() {
        while ( !s_exitApp ) {
            const auto start = std::chrono::high_resolution_clock::now();
            hub::Dof6 dof6( s_pos.x, s_pos.y, s_pos.z, s_quat.w, s_quat.x, s_quat.y, s_quat.z );

#if defined( WIN32 )
            const auto maxFps = 70.0;
#else
            const auto maxFps = 60.0;
#endif
            const auto end = start + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );

            const auto& timestampStart =
                std::chrono::duration_cast<std::chrono::microseconds>( start.time_since_epoch() )
                    .count();
            const auto& timestampEnd =
                std::chrono::duration_cast<std::chrono::microseconds>( end.time_since_epoch() )
                    .count();

            streamer.newAcquisition( sensorName,
                                     std::move( hub::Acquisition( timestampStart, timestampEnd )
                                                << std::move( dof6 ) ) );

            std::this_thread::sleep_until( end );
        }
    } );

    char c;
#ifdef WIN32
    HWND mainWindow = GetForegroundWindow(); // assume here, foregroundWindow is the window of this
                                             // console application
    std::cout << "[main] mainWindow = " << mainWindow << std::endl;
#else
    // Set the terminal to raw mode
    system( "stty raw" );
#endif
    while ( !s_exitApp ) {

#ifdef WIN32
        HWND activeWindow = GetForegroundWindow();
        if ( activeWindow != mainWindow ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            continue;
        }
        bool shifted = false;
        if ( GetKeyState( VK_SHIFT ) & 0x8000 ) {
            // shift down
            shifted = true;
        }
        if ( GetKeyState( VK_OEM_PERIOD ) & 0x8000 || GetKeyState( VK_ESCAPE ) & 0x8000 ) {
            std::cout << std::endl;
            exitApp();
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
                    } // switch getchar() 2
                    break;
                }
                // F5
                else if ( c == 53 ) { init(); }
            } // switch (getchar()) 1
        }
        else if ( c == '.' ) {
            system( "stty cooked" );
            exitApp();
        }
#endif

        if ( s_needUpdate ) {
            s_needUpdate = false;

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
        } // if (s_needUpdate)
    }     // while (1)

    thread.join();

    return 0;
}

//#include <ncurses.h>
////#include <stdio.h>
//#include <iostream>

// int main()
//{

//}

////#define WIDTH 30
////#define HEIGHT 10

//// int startx = 0;
//// int starty = 0;

//// char *choices[] = {
//// int n_choices = sizeof(choices) / sizeof(char *);
//// void print_menu(WINDOW *menu_win, int highlight);

//// int main()
////{

/// c, c); /                refresh(); /                break; /        } / print_menu(menu_win,
/// highlight); /        if(choice != 0)	/* User did a choice come out of the infinite loop */ /
/// break; /    } /    mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice,
/// choices[choice - 1]); /    clrtoeol(); /    refresh(); /    endwin(); /    return 0;
////}
