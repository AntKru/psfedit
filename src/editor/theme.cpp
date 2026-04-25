// SPDX-License-Identifier: GPL-3.0-only

#include "theme.h"

#include <ncurses.h>

#define RGB_TO_NCURSES(x) (int)((float)x * 3.90625)

#define CUSTOM_PRIMARY_BLUE COLOR_WHITE + 1
#define CUSTOM_SECONDARY_BLUE COLOR_WHITE + 2
#define CUSTOM_TERTIARY_BLUE COLOR_WHITE + 3

bool Theme::setTheme(std::string theme) {
    static bool colorsInitialized = false;
    static bool useFallback = false;
    if (!colorsInitialized) {
        int err = 0;
        err |= init_color(CUSTOM_PRIMARY_BLUE, RGB_TO_NCURSES(50), RGB_TO_NCURSES(160), RGB_TO_NCURSES(200));
        err |= init_color(CUSTOM_SECONDARY_BLUE, RGB_TO_NCURSES(20), RGB_TO_NCURSES(20), RGB_TO_NCURSES(30));
        err |= init_color(CUSTOM_TERTIARY_BLUE, RGB_TO_NCURSES(30), RGB_TO_NCURSES(30), RGB_TO_NCURSES(50));
        if (err == ERR) {
            useFallback = true;
        }
        colorsInitialized = true;
    }
    if (useFallback) {
        if (theme == "default") {
            theme = "default-fallback";
        } else  if (theme == "light") {
            theme = "light-fallback";
        }
    }
    if (colorschemes.find(theme) == colorschemes.end()) {
        if (theme != "default-fallback") {
            setTheme("default-fallback");
        }
        return false;
    }
    Colorscheme colorscheme = colorschemes.at(theme);
    for (int i = C_PRIMARY; i < ColorPairs_SIZE; i++) {
        short fg = colorscheme.at(static_cast<ColorPairs>(i)).first;
        short bg = colorscheme.at(static_cast<ColorPairs>(i)).second;
        init_pair(i, fg, bg);
    }
    return true;
}

std::map<std::string, Theme::Colorscheme> Theme::colorschemes = {
    {
        "default",
        {
            {C_PRIMARY, {COLOR_BLACK, CUSTOM_PRIMARY_BLUE}},
            {C_SECONDARY, {COLOR_WHITE, CUSTOM_SECONDARY_BLUE}},
            {C_SECONDARY_IMPORTANT, {COLOR_RED, CUSTOM_SECONDARY_BLUE}},
            {C_TERTIARY, {COLOR_WHITE, CUSTOM_TERTIARY_BLUE}},
            {C_TERTIARY_CURSOR, {COLOR_BLUE, CUSTOM_TERTIARY_BLUE}},
            {C_TERTIARY_MARKER, {COLOR_GREEN, CUSTOM_TERTIARY_BLUE}},
        },
    },
    {
        "default-fallback",
        {
            {C_PRIMARY, {COLOR_BLACK, COLOR_CYAN}},
            {C_SECONDARY, {COLOR_BLACK, COLOR_WHITE}},
            {C_SECONDARY_IMPORTANT, {COLOR_RED, COLOR_WHITE}},
            {C_TERTIARY, {COLOR_WHITE, COLOR_BLACK}},
            {C_TERTIARY_CURSOR, {COLOR_BLUE, COLOR_BLACK}},
            {C_TERTIARY_MARKER, {COLOR_GREEN, COLOR_BLACK}},
        },
    },
    {
        "light",
        {
            {C_PRIMARY, {COLOR_BLACK, CUSTOM_PRIMARY_BLUE}},
            {C_SECONDARY, {COLOR_WHITE, CUSTOM_SECONDARY_BLUE}},
            {C_SECONDARY_IMPORTANT, {COLOR_RED, CUSTOM_SECONDARY_BLUE}},
            {C_TERTIARY, {CUSTOM_TERTIARY_BLUE, COLOR_WHITE}},
            {C_TERTIARY_CURSOR, {COLOR_BLUE, COLOR_WHITE}},
            {C_TERTIARY_MARKER, {COLOR_GREEN, COLOR_WHITE}},
        },
    },
    {
        "light-fallback",
        {
            {C_PRIMARY, {COLOR_BLACK, COLOR_CYAN}},
            {C_SECONDARY, {COLOR_WHITE, COLOR_BLACK}},
            {C_SECONDARY_IMPORTANT, {COLOR_RED, COLOR_BLACK}},
            {C_TERTIARY, {COLOR_BLACK, COLOR_WHITE}},
            {C_TERTIARY_CURSOR, {COLOR_BLUE, COLOR_WHITE}},
            {C_TERTIARY_MARKER, {COLOR_GREEN, COLOR_WHITE}},
        },
    },
    {
        "hacker",
        {
            {C_PRIMARY, {COLOR_BLACK, COLOR_GREEN}},
            {C_SECONDARY, {COLOR_BLACK, COLOR_GREEN}},
            {C_SECONDARY_IMPORTANT, {COLOR_RED, COLOR_GREEN}},
            {C_TERTIARY, {COLOR_GREEN, COLOR_BLACK}},
            {C_TERTIARY_CURSOR, {COLOR_BLUE, COLOR_BLACK}},
            {C_TERTIARY_MARKER, {COLOR_GREEN, COLOR_BLACK}},
        },
    },
};

