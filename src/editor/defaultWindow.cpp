// SPDX-License-Identifier: GPL-3.0-only

#include "defaultWindow.h"
#include "theme.h"

#include <format>
#include <cmath>

void DefaultWindow::update() {
    Window::update();
    wclear(m_win);

    // Status line
    wattron(m_win, COLOR_PAIR(Theme::C_SECONDARY));
    wmove(m_win, m_y - 1, 0);
    wprintw (m_win, "Editor (press \"?\" for help)");
    if (changed()) {
        waddch(m_win, '*' | COLOR_PAIR(Theme::C_SECONDARY_IMPORTANT));
    }
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
    wattron(m_win, COLOR_PAIR(Theme::C_PRIMARY));
    wprintw(m_win, "%s", statusString.c_str());
    wattron(m_win, COLOR_PAIR(Theme::C_TERTIARY));

    // Draw glyph
    if (m_glyph) {
        int y = m_winRootY;
        for (; y < std::min(static_cast<int>(m_glyph->getHeight()), m_y - 1 + m_winRootY); y++) {
            wmove(m_win, y - m_winRootY, 0);
            int x = m_winRootX;
            for (; x < std::min(static_cast<int>(m_glyph->getWidth()), m_x / 2 + m_winRootX); x++) {
                char pixel[] = "  ";
                if (m_borders) {
                    pixel[0] = '[';
                    pixel[1] = ']';
                }
                if (m_glyph->getBit(x, y)) {
                    wattron(m_win, A_REVERSE);
                }
                if (m_vcursorY == y && m_vcursorX == x) {
                    wattron(m_win, COLOR_PAIR(Theme::C_TERTIARY_CURSOR));
                    pixel[0] = '*';
                    pixel[1] = '*';
                }

                if (y == m_marker1Y - 1 && x == m_marker1X - 1) {
                    pixel[0] = '1';
                    wattron(m_win, COLOR_PAIR(Theme::C_TERTIARY_MARKER));
                }
                if (y == m_marker2Y - 1 && x == m_marker2X - 1) {
                    pixel[1] = '2';
                    wattron(m_win, COLOR_PAIR(Theme::C_TERTIARY_MARKER));
                }

                wprintw(m_win, "%s", pixel);
                wattroff(m_win, A_REVERSE);
                wattron(m_win, COLOR_PAIR(Theme::C_TERTIARY));
            }
            if (x < (m_x - 1) / 2) {
                waddch(m_win, '#');
            }
        }
        if (y < m_y - 1) {
            wmove(m_win, y - m_winRootY, 0);
            std::string line(std::min(m_x, static_cast<int>(m_glyph->getWidth() - m_winRootX) * 2 + 1), '#');
            wprintw(m_win, "%s", line.c_str());
        }
    }
}

void DefaultWindow::handleKey(int key) {
    switch (key) {
        // down
        case 'j':
        case KEY_DOWN:
            updateVCursor(1, 0);
            break;
        case 'J':
        case KEY_SF:
            updateVCursor(5, 0);
            break;
        case 10: // ^J
        case 537:
            updateVCursor(10, 0);
            break;
        // up
        case 'k':
        case KEY_UP:
            updateVCursor(-1, 0);
            break;
        case 'K':
        case KEY_SR:
            updateVCursor(-5, 0);
            break;
        case 11: // ^K
        case 578:
            updateVCursor(-10, 0);
            break;
        // left
        case 'h':
        case KEY_LEFT:
            updateVCursor(0, -1);
            break;
        case 'H':
        case KEY_SLEFT:
            updateVCursor(0, -5);
            break;
        case 8: // ^H
        case 557:
            updateVCursor(0, -10);
            break;
        // right
        case 'l':
        case KEY_RIGHT:
            updateVCursor(0, 1);
            break;
        case 'L':
        case KEY_SRIGHT:
            updateVCursor(0, 5);
            break;
        case 12: // ^L
        case 572:
            updateVCursor(0, 10);
            break;

        case 'r':
            m_marker1X = m_marker1Y = 0;
            m_marker2X = m_marker2Y = 0;
            m_winRootX = m_vcursorX = 0;
        case 'g':
            m_winRootY = m_vcursorY = 0;
            break;

        case 'G':
            if (m_glyph) {
                updateVCursor(m_glyph->getHeight() - 1, 0);
            }
            break;

        case 'b':
            m_borders = !m_borders;
            break;

        case ',':
            m_marker1Y = m_vcursorY + 1;
            m_marker1X = m_vcursorX + 1;
            break;

        case '.':
            m_marker2Y = m_vcursorY + 1;
            m_marker2X = m_vcursorX + 1;
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

        case '3':
            if (m_glyph && areMarkersSet()) {
                size_t radius = std::sqrt(std::pow((double)m_marker1X - (double)m_marker2X, 2) + std::pow((double)m_marker1Y - (double)m_marker2Y, 2));;
                try {
                m_glyph->drawCircle(m_marker1X - 1, m_marker1Y - 1, radius, !m_eraser);
                } catch (const std::out_of_range& e) {}
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

        case KEY_MOUSE:
            handleMouse();
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

bool DefaultWindow::changed() {
    return m_history.size() > 1;
}

void DefaultWindow::handleMouse() {
    if (m_glyph) {
        MEVENT event;
        if (getmouse(&event) == OK
            && event.y == std::clamp(event.y, 0, static_cast<int>(m_glyph->getHeight() * 2) - 1)
            && event.x == std::clamp(event.x, 0, static_cast<int>(m_glyph->getWidth() * 2) - 1)
            ) {
            if (event.bstate & BUTTON1_CLICKED) {
                m_marker1Y = event.y + 1 + m_winRootY;
                m_marker1X = event.x / 2 + 1 + m_winRootX;
            }
            if (event.bstate & BUTTON3_CLICKED) {
                m_marker2Y = event.y + 1 + m_winRootY;
                m_marker2X = event.x / 2 + 1 + m_winRootX;
            }
            if (event.bstate & BUTTON2_CLICKED) {
                m_vcursorY = event.y + m_winRootY;
                m_vcursorX = event.x / 2 + m_winRootX;
            }
            if (event.bstate & BUTTON4_PRESSED) {
                if (m_winRootY > 0) {
                    m_winRootY--;
                }
            }
            if (event.bstate & BUTTON5_PRESSED) {
                if (m_winRootY <= static_cast<int>(m_glyph->getHeight() - 1)) {
                    m_winRootY++;
                }
            }
        }
    }
}

bool DefaultWindow::areMarkersSet() {
    return m_marker1X && m_marker1Y && m_marker2X && m_marker2Y;
}

void DefaultWindow::updateVCursor(int y, int x) {
    if (m_glyph) {
        m_vcursorY = std::clamp(m_vcursorY + y, 0, static_cast<int>(m_glyph->getHeight() - 1));
        m_vcursorX = std::clamp(m_vcursorX + x, 0, static_cast<int>(m_glyph->getWidth() - 1));
        m_winRootY = std::clamp(m_winRootY, std::max(0, m_vcursorY - m_y + 2), m_vcursorY);
        m_winRootX = std::clamp(m_winRootX, std::max(0, m_vcursorX - m_x / 2 + 1), m_vcursorX);
    }
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

