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
#pragma GCC diagnostic pop
    std::string statusString = std::format(" {}{}:{} ",
            m_eraser ? "[E] " : "",
            m_vcursorX + 1, m_vcursorY + 1);
    for (size_t i = 0; i < m_x - cursorX - statusString.size(); i++) {
        waddch(m_win, ' ');
    }
    wattron(m_win, COLOR_PAIR(C_UI));
    wprintw(m_win, "%s", statusString.c_str());
    wattroff(m_win, COLOR_PAIR(C_UI));
    wattroff(m_win, A_REVERSE);

    // Draw glyph
    if (m_glyph) {
        int y = m_winRootY;
        for (; y < std::min(static_cast<int>(m_glyph->getHeight()), m_y - 1); y++) {
            wmove(m_win, y, 0);
            int x = m_winRootX;
            for (; x < std::min(static_cast<int>(m_glyph->getWidth()), (m_x - 1) / 2); x++) {
                char pixel[] = "  ";
                if (m_borders) {
                    pixel[0] = '[';
                    pixel[1] = ']';
                }
                if (m_glyph->getBit(x, y)) {
                    wattron(m_win, A_REVERSE);
                }
                if (m_vcursorY == static_cast<size_t>(y) && m_vcursorX == static_cast<size_t>(x)) {
                    wattron(m_win, COLOR_PAIR(C_CURSOR));
                    pixel[0] = '*';
                }

                if (y == m_marker1Y - 1 && x == m_marker1X - 1) {
                    pixel[1] = '1';
                    wattron(m_win, COLOR_PAIR(C_SELECTED));
                }
                if (y == m_marker2Y - 1 && x == m_marker2X - 1) {
                    pixel[1] = '2';
                    wattron(m_win, COLOR_PAIR(C_SELECTED));
                }

                wprintw(m_win, "%s", pixel);
                wattroff(m_win, COLOR_PAIR(C_CURSOR));
                wattroff(m_win, COLOR_PAIR(C_SELECTED));
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
            m_vcursorY = std::min(m_vcursorY + 1, static_cast<size_t>(m_glyph->getHeight() - 1));
            break;
        case 'k':
        case KEY_UP:
            m_vcursorY = std::min(m_vcursorY, m_vcursorY - 1);
            break;
        case 'h':
        case KEY_LEFT:
            m_vcursorX = std::min(m_vcursorX, m_vcursorX - 1);
            break;
        case 'l':
        case KEY_RIGHT:
            m_vcursorX = std::min(m_vcursorX + 1, static_cast<size_t>(m_glyph->getWidth() - 1));
            break;

        case 'r':
            m_vcursorX = m_vcursorY = 0;
            m_winRootX = m_winRootY = 0;
            m_marker1X = m_marker1Y = 0;
            m_marker2X = m_marker2Y = 0;
            break;

        case 'b':
            m_borders = !m_borders;
            break;

        case ' ':
            if (m_currentMarkerIs2) {
                m_marker2Y = m_vcursorY + 1;
                m_marker2X = m_vcursorX + 1;
            } else {
                m_marker1Y = m_vcursorY + 1;
                m_marker1X = m_vcursorX + 1;
            }
            m_currentMarkerIs2 = !m_currentMarkerIs2;
            break;

        case 'e':
            m_eraser = !m_eraser;
            break;

        case 't':
            if (m_glyph) {
                m_glyph->setBit(m_vcursorX, m_vcursorY, !m_glyph->getBit(m_vcursorX, m_vcursorY));
            }
            updateHistory();
            break;

        case '1':
            if (m_glyph && areMarkersSet()) {
                m_glyph->drawLine(m_marker1X - 1, m_marker1Y - 1, m_marker2X - 1, m_marker2Y - 1, !m_eraser);
                updateHistory();
            }
            break;

        case '2':
            if (m_glyph && areMarkersSet()) {
                m_glyph->fill(m_marker1X - 1, m_marker1Y - 1, m_marker2X - 1, m_marker2Y - 1, !m_eraser);
                updateHistory();
            }
            break;

        case '0':
            if (m_glyph) {
                m_glyph->clear(m_eraser);
                updateHistory();
            }
            break;

        case 'u':
            undo();
            break;

        case 'U':
            redo();
            break;
    }
}

void DefaultWindow::setGlyph(const Glyph& glyph) {
    m_glyph = glyph;
    m_history.clear();
    updateHistory();
}

std::optional<Glyph> DefaultWindow::getGlyph() {
    return m_glyph;
}

bool DefaultWindow::areMarkersSet() {
    return m_marker1X && m_marker1Y && m_marker2X && m_marker2Y;
}

void DefaultWindow::updateHistory() {
    if (m_glyph) {
        m_history.push_back(*m_glyph);
        m_redo_history.clear();
    }
}

void DefaultWindow::undo() {
    if (m_history.size() >= 2) {
        m_redo_history.push_back(m_history.back());
        m_history.pop_back();
        m_glyph = m_history.back();
    }
}

void DefaultWindow::redo() {
    if (!m_redo_history.empty()) {
        m_glyph = m_redo_history.back();
        m_redo_history.pop_back();
        m_history.push_back(*m_glyph);
    }
}

