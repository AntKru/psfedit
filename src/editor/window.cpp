// SPDX-License-Identifier: GPL-3.0-only

#include "window.h"

Window::Window() : m_win([]() -> WINDOW* {
        const int y = getmaxy(stdscr);
        const int x = getmaxx(stdscr);
        return newwin(y, x, 0, 0);
    } ()), m_panel(new_panel(m_win)) {
    update();
}

Window::~Window() {
    WINDOW* win = panel_window(m_panel);
    del_panel(m_panel);
    delwin(win);
}

void Window::handleKey(int key) {
}

void Window::update() {
    m_y = getmaxy(stdscr);
    m_x = getmaxx(stdscr);
    wresize(m_win, m_y, m_x);
    wmove(m_win, 0, 0);
}

WINDOW* Window::getWindow() const {
    return m_win;
}

PANEL* Window::getPanel() const {
    return m_panel;
}

