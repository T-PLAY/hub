/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/01
	
#pragma once

#include <string>

namespace hub {
namespace utils {

std::string getHostname();

enum Key {
    // ASCII Characters
    NUL = 0,          // 0
    SOH,              // 1
    STX,              // 2
    ETX,              // 3
    EOT,              // 4
    ENQ,              // 5
    ACK,              // 6
    BEL,              // 7
    BS,               // 8
    HT,               // 9
    LF,               // 10
    VT,               // 11
    FF,               // 12
    CR,               // 13
    SO,               // 14
    SI,               // 15
    DLE,              // 16
    DC1,              // 17
    DC2,              // 18
    DC3,              // 19
    DC4,              // 20
    NAK,              // 21
    SYN,              // 22
    ETB,              // 23
    CAN,              // 24
    EM,               // 25
    SUB,              // 26
    ESC,              // 27
    FS,               // 28
    GS,               // 29
    RS,               // 30
    US,               // 31
    Space,            // 32
    Exclamation,      // 33
    DoubleQuote,      // 34
    Sharp,            // 35
    $,                // 36
    Percent,          // 37
    And,              // 38
    Quote,            // 39
    LeftParenthesis,  // 40
    RightParenthesis, // 41
    Star,             // 42
    Plus,             // 43
    Comma,            // 44
    Minus,            // 45
    Dot,              // 46
    Slash,            // 47
    Zero,             // 48
    One,              // 49
    Two,              // 50
    Three,            // 51
    Four,             // 52
    Five,             // 53
    Six,              // 54
    Seven,            // 55
    Eight,            // 56
    Nine,             // 57
    DoublePoint,      // 58
    Semicolon,        // 59
    Less,             // 60
    Equal,            // 61
    More,             // 62
    Interrogation,    // 63
    At,               // 64
    A,                // 65
    B,                // 66
    C,                // 67
    D,                // 68
    E,                // 69
    F,                // 70
    G,                // 71
    H,                // 72
    I,                // 73
    J,                // 74
    K,                // 75
    L,                // 76
    M,                // 77
    N,                // 78
    O,                // 79
    P,                // 80
    Q,                // 81
    R,                // 82
    S,                // 83
    T,                // 84
    U,                // 85
    V,                // 86
    W,                // 87
    X,                // 88
    Y,                // 89
    Z,                // 90
    LeftBracket,      // 91
    AntiSlash,        // 92
    RightBracket,     // 93
    Hat,              // 94
    _,                // 95
    GraveAccent,      // 96
    a,                // 97
    b,                // 98
    c,                // 99
    d,                // 100
    e,                // 101
    f,                // 102
    g,                // 103
    h,                // 104
    i,                // 105
    j,                // 106
    k,                // 107
    l,                // 108
    m,                // 109
    n,                // 110
    o,                // 111
    p,                // 112
    q,                // 113
    r,                // 114
    s,                // 115
    t,                // 116
    u,                // 117
    v,                // 118
    w,                // 119
    x,                // 120
    y,                // 121
    z,                // 122
    LeftBrace,        // 123
    Pipe,             // 124
    RightBrace,       // 125
    Tilde,            // 126
    DEL,              // 127

    // Non ASCII Characters
    Backspace,
    Escape,
    Delete,
    UpArrow,
    DownArrow,
    RightArrow,
    LeftArrow,
    PageUp,
    PageDown,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    Ctrl,
    Shift,

    Count
};

Key key_press();
std::ostream& operator<<( std::ostream& os, Key key );

} // namespace utils
} // namespace hub
