// SPDX-License-Identifier: GPL-3.0-only

#include "overviewWindow.h"

void OverviewWindow::update() {
    Window::update();
    size_t win_y = m_y - 4;
    size_t win_x = m_x - 4;
    wresize(m_win, win_y, win_x);
    move_panel(m_panel, 2, 2);
    box(m_win, 0, 0);
    mvwprintw(m_win, 0, 1, "Overview");
    if (m_glyph) {
        size_t currentY = 0;
        for (size_t y = m_cursorY; y < std::min(static_cast<size_t>(m_glyph->getHeight()), (win_y - 2) * 2); y += 2) {
            wmove(m_win, currentY + 1, 1);
            currentY++;
            size_t x = 0;
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
            if (x < win_x - 2) {
                wprintw(m_win, "#");
            }
        }
        if (currentY < win_y - 2) {
            wmove(m_win, currentY + 1, 1);
            for (size_t x = 0; x < std::min(static_cast<size_t>(m_glyph->getWidth() + 1), win_x - 2); x++) {
                wprintw(m_win, "#");
            }
        }
    }
}

void OverviewWindow::handleKey(int key) {
}

void OverviewWindow::setGlyph(const Glyph& glyph) {
    m_glyph = glyph;
}

