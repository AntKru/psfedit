// SPDX-License-Identifier: GPL-3.0-only

#include "overviewWindow.h"

#include <algorithm>

void OverviewWindow::update() {
    Window::update();
    size_t win_y = m_y - 4;
    size_t win_x = m_x - 4;
    wresize(m_win, win_y, win_x);
    move_panel(m_panel, 2, 2);
    wclear(m_win);
    box(m_win, 0, 0);
    mvwprintw(m_win, 0, 1, "Overview");
    if (m_glyph) {
        size_t currentY = 0;
        for (size_t y = m_cursorY; y < static_cast<size_t>(m_glyph->getHeight()) && currentY < win_y - 2; y += 2) {
            wmove(m_win, currentY + 1, 1);
            currentY++;
            size_t x = m_cursorX;
            for (; x < std::min(static_cast<size_t>(m_glyph->getWidth()), win_x - 2); x++) {
                bool bit = m_glyph->getBit(x, y);
                if (bit) {
                    wattron(m_win, A_REVERSE);
                } else {
                    wattroff(m_win, A_REVERSE);
                }
                if (y + 1 >= m_glyph->getHeight()) {
                    wprintw(m_win, "%s", bit ? "▄" : " ");
                } else {
                    wprintw(m_win, "%s", bit == m_glyph->getBit(x, y + 1) ? " " : "▄");
                }
            }
            if (x - m_cursorX < win_x - 2) {
                wprintw(m_win, "#");
            }
        }
        if (currentY < win_y - 2) {
            wmove(m_win, currentY + 1, 1);
            for (size_t x = m_cursorX; x < std::min(static_cast<size_t>(m_glyph->getWidth() + 1), win_x - 2); x++) {
                wprintw(m_win, "#");
            }
        }
    }
}

void OverviewWindow::handleKey(int key) {
    if (m_glyph) {
        switch (key) {
            case 'j':
            case KEY_DOWN:
                m_cursorY = std::min(m_cursorY + 1, static_cast<size_t>(m_glyph->getHeight()));
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
                m_cursorX = std::min(m_cursorX + 1, static_cast<size_t>(m_glyph->getWidth()));
                break;

            case 'r':
                m_cursorX = m_cursorY = 0;
        }
    }
}

void OverviewWindow::setGlyph(const Glyph& glyph) {
    m_glyph = glyph;
}

