// SPDX-License-Identifier: GPL-3.0-only

#include "helpWindow.h"

void HelpWindow::update() {
    Window::update();
    wresize(m_win, m_y - 4, m_x - 4);
    move_panel(m_panel, 2, 2);
    box(m_win, 0, 0);
    mvwprintw(m_win, 0, 1, "Help");
}

