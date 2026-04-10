// SPDX-License-Identifier: GPL-3.0-only

#include "helpWindow.h"

PANEL* HelpWindow::create() {
    int y, x;
    getmaxyx(stdscr, y, x);
    WINDOW* win = newwin(y - 4, x - 4, 2, 2);
    box(win, 0, 0);
    wprintw(win, "Help");
    return new_panel(win);
}

