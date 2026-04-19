// SPDX-License-Identifier: GPL-3.0-only

#include "saveWindow.h"

void SaveWindow::update() {
    Window::update();
    wresize(m_win, m_message.size() + 2, m_messageWidth + 2);
    move_panel(m_panel, (m_y - m_message.size() + 2) / 2, (m_x - m_messageWidth + 2) / 2);
    box(m_win, 0, 0);
    for (size_t i = 0; i < m_message.size(); i++) {
        wmove(m_win, i + 1, 1);
        wprintw(m_win, "%s", m_message.at(i).c_str());
    }
}

void SaveWindow::handleKey(int key) {
    switch (key) {
        case 'y':
            m_action = Action::SAVE_QUIT;
            break;

        case 'n':
            m_action = Action::FORCE_QUIT;
            break;
    }
}

SaveWindow::Action SaveWindow::getAction() {
    return m_action;
}

void SaveWindow::setAction(Action action) {
    m_action = action;
}

