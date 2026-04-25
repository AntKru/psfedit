// SPDX-License-Identifier: GPL-3.0-only

#include "theme.h"

#include <ncurses.h>


bool Theme::setTheme(const std::string& theme) {
    if (colorschemes.find(theme) == colorschemes.end()) {
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

std::unordered_map<std::string, Theme::Colorscheme> Theme::colorschemes = {
    {
        "default",
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
            {C_PRIMARY, {COLOR_BLACK, COLOR_CYAN}},
            {C_SECONDARY, {COLOR_WHITE, COLOR_BLACK}},
            {C_SECONDARY_IMPORTANT, {COLOR_RED, COLOR_BLACK}},
            {C_TERTIARY, {COLOR_BLACK, COLOR_WHITE}},
            {C_TERTIARY_CURSOR, {COLOR_BLUE, COLOR_WHITE}},
            {C_TERTIARY_MARKER, {COLOR_GREEN, COLOR_WHITE}},
        },
    },
};

