#pragma once

#include <cstdio>
#include <cstring>

enum ANSICode{
    Reset = 0,

    //Text Styles
    Bold = 1,
    Faint = 2,
    Italic = 3,
    Underline = 4,
    SlowBlink = 5,
    RapidBlink = 6,
    Swap = 7,
    Conceal = 8,
    CrossedOut = 9,

    //Text Color
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    White = 37,

    // Background Colors
    BackgroundBlack = 40,
    BackgroundRed = 41,
    BackgroundGreen = 42,
    BackgroundYellow = 43,
    BackgroundBlue = 44,
    BackgroundMagenta = 45,
    BackgroundCyan = 46,
    BackgroundWhite = 47,

    //Bright Colors
    BrightBlack = 90,
    BrightRed = 91,
    BrightGreen = 92,
    BrightYellow = 93,
    BrightBlue = 94,
    BrightMagenta = 95,
    BrightCyan = 96,
    BrightWhite = 97,

    // Bright Background Colors
    BackgroundBrightBlack = 100,
    BackgroundBrightRed = 101,
    BackgroundBrightGreen = 102,
    BackgroundBrightYellow = 103,
    BackgroundBrightBlue = 104,
    BackgroundBrightMagenta = 105,
    BackgroundBrightCyan = 106,
    BackgroundBrightWhite = 107
};

static inline const char* ANSICodeToCStr(ANSICode code)
{
    unsigned int _code = static_cast<unsigned int>(code);
    static char escapeSequence[32];
    snprintf(escapeSequence, sizeof(escapeSequence), "\x1B[%dm", _code);
    return escapeSequence;
}

template<typename ...Codes>
static inline const char* ANSICodesToCStr(Codes... codes) {
    ANSICode _codes[] = { codes... };
    size_t codes_count = sizeof(_codes) / sizeof(_codes[0]);
    if (codes_count == 0) {
        return "";
    }

    static char escapeSequence[256];
    snprintf(escapeSequence, sizeof(escapeSequence), "\x1B[%d", _codes[0]);

    for (size_t i = 1; i < codes_count; ++i) {
        snprintf(escapeSequence + strlen(escapeSequence), sizeof(escapeSequence) - strlen(escapeSequence), ";%d", _codes[i]);
    }

    snprintf(escapeSequence + strlen(escapeSequence), sizeof(escapeSequence) - strlen(escapeSequence), "m");
    return escapeSequence;
}