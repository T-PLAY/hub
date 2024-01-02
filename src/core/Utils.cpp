#include "Utils.hpp"

// #include <iostream>
#include <chrono>
// #include <memory>
#include <string>
#ifndef WIN32
#    include <unistd.h>
#else
#    include <iostream>
#    include <winsock2.h>
// #include <WinBase.h>
#endif

// #include "Macros.hpp"
//  #include "Types.hpp"

#include <iostream>
#include <string>
#include <thread> // contains <chrono>
#include <cassert>

namespace hub {
namespace utils {

std::string getHostname() {
    char* temp = 0;
    std::string computerName;

#if defined( WIN32 ) || defined( _WIN32 ) || defined( _WIN64 )
    //    temp = getenv( "COMPUTERNAME" );
    //    char buff[256];
    // LPDWORD len;
    // int err = _dupenv_s((char**)&buff, &len, "COMPUTERNAME");
    // int err = gethostname( buff, 256 );
    // GetComputerNameA( buff, &len );

    WORD wVersionRequested;
    WSADATA wsaData;
    char name[255];
    // CString ip;
    std::string ip;
    //    PHOSTENT hostinfo;
    wVersionRequested = MAKEWORD( 2, 0 );

    if ( WSAStartup( wVersionRequested, &wsaData ) == 0 ) {
        if ( gethostname( name, sizeof( name ) ) != 0 ) {
            assert( false );
            // if ( ( hostinfo = gethostbyname( name ) ) != NULL ) {
            //     ip = inet_ntoa( *(struct in_addr*)*hostinfo->h_addr_list );
            // }
        }
        WSACleanup();
    }

    // std::cout << "hostname = " << name << std::endl;

    // assert(len < 256);
    // assert(err == 0);
    computerName = name;
//    if ( temp != 0 ) {
//        computerName = temp;
//        temp         = 0;
//    }
#else
    temp = getenv( "HOSTNAME" );
    if ( temp != 0 ) {
        computerName = temp;
        temp         = 0;
    }
    else {
        temp = new char[512];
        if ( gethostname( temp, 512 ) == 0 ) { // success = 0, failure = -1
            computerName = temp;
        }
        delete[] temp;
        temp = 0;
    }
#endif
    return computerName;
}

// using namespace std;

// void println(const string& s="") {
//     cout << s << endl;
// }
void sleep( const double t ) {
    if ( t > 0.0 )
        std::this_thread::sleep_for( std::chrono::milliseconds( (int)( 1E3 * t + 0.5 ) ) );
}

// struct Key {
// ASCII codes (key>0): 8 backspace, 9 tab, 10 newline, 27 escape, 127 delete,
// !"#$%&'()*+,-./0-9:;<=>?@A-Z[]^_`a-z{|}~üäÄöÖÜßµ´§°¹³² control key codes (key<0): -38/-40/-37/-39
// up/down/left/right arrow, -33/-34 page up/down, -36/-35 pos1/end other key codes (key<0): -45
// insert, -144 num lock, -20 caps lock, -91 windows key, -93 kontext menu key, -112 to -123 F1 to
// F12 not working: ¹ (251), num lock (-144), caps lock (-20), windows key (-91), kontext menu key
// (-93), F11 (-122)
//#if defined( _WIN32 )
#if defined( WIN32 ) || defined( _WIN32 ) || defined( _WIN64 )
#    define WIN32_LEAN_AND_MEAN
#    define VC_EXTRALEAN
#    include <Windows.h>
Key key_press() { // not working: F11 (-122, toggles fullscreen)
    KEY_EVENT_RECORD keyevent;
    INPUT_RECORD irec;
    DWORD events;
    while ( true ) {
        ReadConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &irec, 1, &events );
        if ( irec.EventType == KEY_EVENT && ( (KEY_EVENT_RECORD&)irec.Event ).bKeyDown ) {
            keyevent      = (KEY_EVENT_RECORD&)irec.Event;
            const int ca  = (int)keyevent.uChar.AsciiChar;
            const int cv  = (int)keyevent.wVirtualKeyCode;
            const int key = ca == 0 ? -cv : ca + ( ca > 0 ? 0 : 256 );
            switch ( key ) {
            case -16:
                continue; // disable Shift
            case -17:
                continue; // disable Ctrl / AltGr
            case -18:
                continue; // disable Alt / AltGr
            case -220:
                continue; // disable first detection of "^" key (not "^" symbol)
            case -221:
                continue; // disable first detection of "`" key (not "`" symbol)
            case -191:
                continue; // disable AltGr + "#"
            case -52:
                continue; // disable AltGr + "4"
            case -53:
                continue; // disable AltGr + "5"
            case -54:
                continue; // disable AltGr + "6"
            case -12:
                continue; // disable num block 5 with num lock deactivated
            //case 13:
            //    return 10; // enter
            case -46:
                return Key::Delete;
                //return 127; // delete
            //case -49:
            //    return 251; // ¹
            case 0:
                continue;
            case 1:
                continue; // disable Ctrl + a (selects all text)
            case 2:
                continue; // disable Ctrl + b
            case 3:
                continue; // disable Ctrl + c (terminates program)
            case 4:
                continue; // disable Ctrl + d
            case 5:
                continue; // disable Ctrl + e
            case 6:
                continue; // disable Ctrl + f (opens search)
            case 7:
                continue; // disable Ctrl + g
            // case    8: continue; // disable Ctrl + h (ascii for backspace)
            // case    9: continue; // disable Ctrl + i (ascii for tab)
            case 10:
                continue; // disable Ctrl + j
            case 11:
                continue; // disable Ctrl + k
            case 12:
                continue; // disable Ctrl + l
            // case   13: continue; // disable Ctrl + m (breaks console, ascii for new line)
            case 14:
                continue; // disable Ctrl + n
            case 15:
                continue; // disable Ctrl + o
            case 16:
                continue; // disable Ctrl + p
            case 17:
                continue; // disable Ctrl + q
            case 18:
                continue; // disable Ctrl + r
            case 19:
                continue; // disable Ctrl + s
            case 20:
                continue; // disable Ctrl + t
            case 21:
                continue; // disable Ctrl + u
            case 22:
                continue; // disable Ctrl + v (inserts clipboard)
            case 23:
                continue; // disable Ctrl + w
            case 24:
                continue; // disable Ctrl + x
            case 25:
                continue; // disable Ctrl + y
            case 26:
                continue; // disable Ctrl + z
            default:
                return (Key)key; // any other ASCII/virtual character
            }
        }
    }
}
//#elif defined( __linux__ )
#else
#    include <sys/ioctl.h>
#    include <termios.h>
Key key_press() { // not working: ¹ (251), num lock (-144), caps lock (-20), windows key
                         // (-91), kontext menu key (-93)
    struct termios term;
    tcgetattr( 0, &term );
    while ( true ) {
        term.c_lflag &= ~( ICANON | ECHO ); // turn off line buffering and echoing
        tcsetattr( 0, TCSANOW, &term );
        int nbbytes;
        ioctl( 0, FIONREAD, &nbbytes ); // 0 is STDIN
        while ( !nbbytes ) {
            sleep( 0.01 );
            fflush( stdout );
            ioctl( 0, FIONREAD, &nbbytes ); // 0 is STDIN
        }
        int key = (int)getchar();
        if ( key == 27 || key == 194 || key == 195 ) { // escape, 194/195 is escape for °ß´äöüÄÖÜ
            key = (int)getchar();
            if ( key == 91 ) {                 // [ following escape
                key = (int)getchar();          // get code of next char after \e[
                if ( key == 49 ) {             // F5-F8
                    key = 62 + (int)getchar(); // 53, 55-57
                    if ( key == 115 ) key++;   // F5 code is too low by 1
                    getchar();                 // take in following ~ (126), but discard code
                }
                else if ( key == 50 ) { // insert or F9-F12
                    key = (int)getchar();
                    if ( key == 126 ) { // insert
                        key = 45;
                    }
                    else {                      // F9-F12
                        key += 71;              // 48, 49, 51, 52
                        if ( key < 121 ) key++; // F11 and F12 are too low by 1
                        getchar();              // take in following ~ (126), but discard code
                    }
                }
                else if ( key == 51 || key == 53 || key == 54 ) { // delete, page up/down
                    getchar(); // take in following ~ (126), but discard code
                }
            }
            else if ( key == 79 ) {        // F1-F4
                key = 32 + (int)getchar(); // 80-83
            }
            key = -key; // use negative numbers for escaped keys
        }
        term.c_lflag |= ( ICANON | ECHO ); // turn on line buffering and echoing
        tcsetattr( 0, TCSANOW, &term );
        // std::cout << "input key"
        // std::cout << "[Utils] Input is: " << std::to_string( key ) << ", \"" << (char)key << "\""
        // << std::endl;

        switch ( key ) {
        case 127:
            return Key::Backspace;
            // return 8; // backspace
        case -27:
            // return 27; // escape
            return Key::Escape;
        case -51:
            // return 127; // delete
            return Key::Delete;
        case -112: // F1
        case -113: // F2
        case -114: // F3
        case -115: // F4
        case -116: // F5
        case -117: // F6
        case -118: // F7
        case -119: // F8
        case -120: // F9
            return Key( -key - 112 + (int)Key::F1 );

        // case -164:
        //     return 132; // ä
        // case -182:
        //     return 148; // ö
        // case -188:
        //     return 129; // ü
        // case -132:
        //     return 142; // Ä
        // case -150:
        //     return 153; // Ö
        // case -156:
        //     return 154; // Ü
        // case -159:
        //     return 225; // ß
        // case -181:
        //     return 230; // µ
        // case -167:
        //     return 245; // §
        // case -176:
        //     return 248; // °
        // case -178:
        //     return 253; // ²
        // case -179:
        //     return 252; // ³
        // case -180:
        //     return 239; // ´
        case -65:
            // return -38; // up arrow
            return Key::UpArrow;
        case -66:
            // return -40; // down arrow
            return Key::DownArrow;
        case -68:
            // return -37; // left arrow
            return Key::LeftArrow;
        case -67:
            // return -39; // right arrow
            return Key::RightArrow;
        case -53:
            // return -33; // page up
            return Key::PageUp;
        case -54:
            // return -34; // page down
            return Key::PageDown;
        // case -72:
        // return -36; // pos1
        // case -70:
        // return -35; // end
        case 0:
            continue;
        case 1:
            continue; // disable Ctrl + a
        case 2:
            continue; // disable Ctrl + b
        case 3:
            continue; // disable Ctrl + c (terminates program)
        case 4:
            continue; // disable Ctrl + d
        case 5:
            continue; // disable Ctrl + e
        case 6:
            continue; // disable Ctrl + f
        case 7:
            continue; // disable Ctrl + g
        case 8:
            continue; // disable Ctrl + h
        // case    9: continue; // disable Ctrl + i (ascii for tab)
        // case   10: continue; // disable Ctrl + j (ascii for new line)
        case 11:
            continue; // disable Ctrl + k
        case 12:
            continue; // disable Ctrl + l
        case 13:
            continue; // disable Ctrl + m
        case 14:
            continue; // disable Ctrl + n
        case 15:
            continue; // disable Ctrl + o
        case 16:
            continue; // disable Ctrl + p
        case 17:
            continue; // disable Ctrl + q
        case 18:
            continue; // disable Ctrl + r
        case 19:
            continue; // disable Ctrl + s
        case 20:
            continue; // disable Ctrl + t
        case 21:
            continue; // disable Ctrl + u
        case 22:
            continue; // disable Ctrl + v
        case 23:
            continue; // disable Ctrl + w
        case 24:
            continue; // disable Ctrl + x
        case 25:
            continue; // disable Ctrl + y
        case 26:
            continue; // disable Ctrl + z (terminates program)
        default:
            return (Key)key; // any other ASCII character
        }
    }
}
#endif // Windows/Linux

static constexpr std::string_view s_key2str[Key::Count] = {
    // ASCII Characters
    "NUL",              // 0
    "SOH",              // 1
    "STX",              // 2
    "ETX",              // 3
    "EOT",              // 4
    "ENQ",              // 5
    "ACK",              // 6
    "BEL",              // 7
    "BS",               // 8
    "HT",               // 9
    "LF",               // 10
    "VT",               // 11
    "FF",               // 12
    "CR",               // 13
    "SO",               // 14
    "SI",               // 15
    "DLE",              // 16
    "DC1",              // 17
    "DC2",              // 18
    "DC3",              // 19
    "DC4",              // 20
    "NAK",              // 21
    "SYN",              // 22
    "ETB",              // 23
    "CAN",              // 24
    "EM",               // 25
    "SUB",              // 26
    "ESC",              // 27
    "FS",               // 28
    "GS",               // 29
    "RS",               // 30
    "US",               // 31
    "Space",            // 32
    "Exclamation",      // 33
    "DoubleQuote",      // 34
    "Sharp",            // 35
    "$",                // 36
    "Percent",          // 37
    "And",              // 38
    "Quote",            // 39
    "LeftParenthesis",  // 40
    "RightParenthesis", // 41
    "Star",             // 42
    "Plus",             // 43
    "Comma",            // 44
    "Minus",            // 45
    "Dot",              // 46
    "Slash",            // 47
    "Zero",             // 48
    "One",              // 49
    "Two",              // 50
    "Three",            // 51
    "Four",             // 52
    "Five",             // 53
    "Six",              // 54
    "Seven",            // 55
    "Eight",            // 56
    "Nine",             // 57
    "DoublePoint",      // 58
    "Semicolon",        // 59
    "Less",             // 60
    "Equal",            // 61
    "More",             // 62
    "Interrogation",    // 63
    "At",               // 64
    "A",                // 65
    "B",                // 66
    "C",                // 67
    "D",                // 68
    "E",                // 69
    "F",                // 70
    "G",                // 71
    "H",                // 72
    "I",                // 73
    "J",                // 74
    "K",                // 75
    "L",                // 76
    "M",                // 77
    "N",                // 78
    "O",                // 79
    "P",                // 80
    "Q",                // 81
    "R",                // 82
    "S",                // 83
    "T",                // 84
    "U",                // 85
    "V",                // 86
    "W",                // 87
    "X",                // 88
    "Y",                // 89
    "Z",                // 90
    "LeftBracket",      // 91
    "AntiSlash",        // 92
    "RightBracket",     // 93
    "Hat",              // 94
    "_",                // 95
    "GraveAccent",      // 96
    "a",                // 97
    "b",                // 98
    "c",                // 99
    "d",                // 100
    "e",                // 101
    "f",                // 102
    "g",                // 103
    "h",                // 104
    "i",                // 105
    "j",                // 106
    "k",                // 107
    "l",                // 108
    "m",                // 109
    "n",                // 110
    "o",                // 111
    "p",                // 112
    "q",                // 113
    "r",                // 114
    "s",                // 115
    "t",                // 116
    "u",                // 117
    "v",                // 118
    "w",                // 119
    "x",                // 120
    "y",                // 121
    "z",                // 122
    "LeftBrace",        // 123
    "Pipe",             // 124
    "RightBrace",       // 125
    "Tilde",            // 126
    "DEL",              // 127

    // Non ASCII Characters
    "Backspace",
    "Escape",
    "Delete",
    "UpArrow",
    "DownArrow",
    "RightArrow",
    "LeftArrow",
    "PageUp",
    "PageDown",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "Ctrl",
    "Shift",
};

std::ostream& operator<<( std::ostream& os, Key key ) {
    os << s_key2str[(int)key];
    return os;
}

// std::ostream &operator<<(std::ostream &os, Key key) {
//     os << s_key2str[(int)key];
//     return os;
// }

} // namespace utils
} // namespace hub
