// SPDX-License-Identifier: GPL-3.0-only

#include "defaultWindow.h"
#include <format>

void DefaultWindow::update() {
    Window::update();

    // Status line
    wattron(m_win, A_REVERSE);
    wmove(m_win, m_y - 1, 0);
    wprintw (m_win, "Editor (press \"?\" for help)");
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    int cursorY, cursorX;
    getyx(m_win, cursorY, cursorX);
    std::string statusString = std::format("{}{}:{} ",
            m_glyph ? "glyph loaded " : "",
            m_cursorX, m_cursorY);
#pragma GCC diagnostic pop
    for (size_t i = 0; i < m_x - cursorX - statusString.size(); i++) {
        waddch(m_win, ' ');
    }
    wprintw(m_win, "%s", statusString.c_str());
    wattroff(m_win, A_REVERSE);

    // Draw glyph
    if (m_glyph) {
        int y = m_winRootY;
        for (; y < std::min(static_cast<int>(m_glyph->getHeight()), m_y - 1); y++) {
            wmove(m_win, y, 0);
            int x = m_winRootX;
            for (; x < std::min(static_cast<int>(m_glyph->getWidth()), (m_x - 1) / 2); x++) {
                if (m_glyph->getBit(x, y)) {
                    wattron(m_win, A_REVERSE);
                }
                if (m_cursorY == static_cast<size_t>(y) && m_cursorX == static_cast<size_t>(x)) {
                    wprintw(m_win, "* ");
                } else if (m_highlight){
                    wprintw(m_win, "[]");
                } else {
                    wprintw(m_win, "  ");
                }
                wattroff(m_win, A_REVERSE);
            }
            if (x < (m_x - 1) / 2) {
                waddch(m_win, '#');
            }
        }
        if (y < m_y - 1) {
            wmove(m_win, y, 0);
            std::string line(std::min(m_x, static_cast<int>(m_glyph->getWidth() - m_winRootX) * 2 + 1), '#');
            wprintw(m_win, "%s", line.c_str());
        }
    }
}

void DefaultWindow::handleKey(int key) {
    switch (key) {
        case 'j':
        case KEY_DOWN:
            m_cursorY = std::min(m_cursorY + 1, static_cast<size_t>(m_glyph->getHeight() - 1));
            break;
        case 'k':
        case KEY_UP:
            m_cursorY = std::min(m_cursorY, m_cursorY - 1);
            break;
        case 'h':
        case KEY_LEFT:
            m_cursorX = std::min(m_cursorX, m_cursorX - 1);
            break;
        case 'l':
        case KEY_RIGHT:
            m_cursorX = std::min(m_cursorX + 1, static_cast<size_t>(m_glyph->getWidth() - 1));
            break;

        case 'r':
            m_cursorX = m_cursorY = 0;
            m_winRootX = m_winRootY = 0;

        case 'b':
            m_highlight = !m_highlight;
    }
}

void DefaultWindow::setGlyph(const Glyph& glyph) {
    m_glyph = glyph;
}

