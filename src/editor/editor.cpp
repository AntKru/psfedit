// SPDX-License-Identifier: GPL-3.0-only

#include <string>

#include "editor.h"
#include "defaultWindow.h"
#include "helpWindow.h"

Editor::Editor() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    std::string message = "Editor initialized, waiting for glyph";
    WINDOW* messageWin = newwin(3, message.size() + 2, 0, 0);
    box(messageWin, 0, 0);
    mvwprintw(messageWin, 1, 1, "%s", message.c_str());
    wrefresh(messageWin);
    m_windows.at(DEFAULT_PANEL) = std::make_unique<DefaultWindow>();
    m_windows.at(HELP_PANEL) = std::make_unique<HelpWindow>();
}

Editor::~Editor() {
    endwin();
}

std::optional<Glyph> Editor::editGlyph(Glyph glyph) {
    clear();
    ActiveWindow activeWindow = DEFAULT_PANEL;
    ActiveWindow lastActiveWindow = DEFAULT_PANEL;
    setActiveWindow(*m_windows.at(DEFAULT_PANEL));

    while (true) {
        for (const auto& windowp : m_windows) {
            if (windowp) {
                windowp->update();
            }
        }
        update_panels();
        doupdate();
        int ch = wgetch(m_windows.at(activeWindow)->getWindows().front());
        switch (ch) {
            case 'h':
                if (activeWindow != HELP_PANEL) {
                    lastActiveWindow = activeWindow;
                    activeWindow = HELP_PANEL;
                    setActiveWindow(*m_windows.at(activeWindow));
                }
                break;
            case 'q':
                if (activeWindow == DEFAULT_PANEL) {
                    return {};
                }
                if (activeWindow != lastActiveWindow) {
                    ActiveWindow old = activeWindow;
                    activeWindow = lastActiveWindow;
                    lastActiveWindow = old;
                    setActiveWindow(*m_windows.at(activeWindow));
                }
                break;
            default:
                m_windows.at(activeWindow)->handleKey(ch);
                break;
        }
    }
}

void Editor::setActiveWindow(const Window& window) {
    for (PANEL* panel : window.getPanels()) {
        top_panel(panel);
    }
}

