// SPDX-License-Identifier: GPL-3.0-only

#include "defaultWindow.h"

void DefaultWindow::update() {
    Window::update();
    wattron(m_win, A_REVERSE);
    wmove(m_win, 0, 0);
    wprintw (m_win, "Editor (press \"?\" for help)");
    int cursorY, cursorX;
    getyx(m_win, cursorY, cursorX);
    while (cursorY == 0) {
        waddch(m_win, ' ');
        getyx(m_win, cursorY, cursorX);
    }
    wattroff(m_win, A_REVERSE);
}

