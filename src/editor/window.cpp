// SPDX-License-Identifier: GPL-3.0-only

#include "window.h"

Window::Window()
    : m_wins({newwin(0, 0, 0, 0)}),
      m_panels({new_panel(m_wins.front())}),
      m_win(m_wins.front()),
      m_panel(m_panels.front()) {
    static bool pairsInitialized = false;
    if (!pairsInitialized) {
        init_pair(C_SELECTED, COLOR_GREEN, COLOR_BLACK);
        init_pair(C_CURSOR, COLOR_BLUE, COLOR_BLACK);
        init_pair(C_UI, COLOR_CYAN, COLOR_BLACK);
        init_pair(C_IMPORTANT, COLOR_WHITE, COLOR_RED);
        pairsInitialized = true;
    }
    keypad(m_win, true);
    update();
}

Window::~Window() {
    for (size_t i = 0; i < m_panels.size(); i++) {
        del_panel(m_panels.at(i));
        delwin(m_wins.at(i));
    }
}

void Window::handleKey(int key) {
}

void Window::update() {
    m_y = getmaxy(stdscr);
    m_x = getmaxx(stdscr);
    wresize(m_win, m_y, m_x);
    wmove(m_win, 0, 0);
}

std::vector<WINDOW*> Window::getWindows() const {
    return m_wins;
}

std::vector<PANEL*> Window::getPanels() const {
    return m_panels;
}

void Window::pushWindow(WINDOW* win) {
    m_wins.push_back(win);
    m_panels.push_back(new_panel(win));
}

bool Window::popWindow() {
    if (m_panels.size() <= 1) {
        return false;
    }
    m_wins.pop_back();
    m_panels.pop_back();
    return true;
}

