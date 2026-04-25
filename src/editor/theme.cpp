// SPDX-License-Identifier: GPL-3.0-only

#include "theme.h"

#include <ncurses.h>

bool Theme::setTheme(const Theme& theme) {
    return false;
}

void Theme::resetTheme() {
    init_pair(C_PRIMARY, COLOR_BLACK, COLOR_CYAN);
    init_pair(C_SECONDARY, COLOR_BLACK, COLOR_WHITE);
    init_pair(C_SECONDARY_IMPORTANT, COLOR_RED, COLOR_WHITE);
    init_pair(C_TERTIARY, COLOR_RED, COLOR_BLACK);
    init_pair(C_TERTIARY_CURSOR, COLOR_BLUE, COLOR_BLACK);
    init_pair(C_TERTIARY_MARKER, COLOR_GREEN, COLOR_BLACK);
}

short Theme::currentColorIndex = COLOR_WHITE + 1;

