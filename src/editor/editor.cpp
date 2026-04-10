// SPDX-License-Identifier: GPL-3.0-only

#include <string>

#include "editor.h"
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
    m_panels[DEFAULT_PANEL] = createDefaultPanel();
    m_panels[HELP_PANEL] = HelpWindow::create();
}

Editor::~Editor() {
    for (size_t i = 0; i < ActiveWindowSIZE; i++) {
        WINDOW* win = panel_window(m_panels[i]);
        del_panel(m_panels[i]);
        delwin(win);
    }
    refresh();
    endwin();
}

std::optional<Glyph> Editor::editGlyph(Glyph glyph) {
    clear();
    ActiveWindow activeWindow = DEFAULT_PANEL;
    ActiveWindow lastActiveWindow = DEFAULT_PANEL;
    top_panel(m_panels[DEFAULT_PANEL]);

    while (true) {
        update_panels();
        doupdate();
        int ch = wgetch(panel_window(m_panels[activeWindow]));
        switch (ch) {
            case 'h':
                if (activeWindow != HELP_PANEL) {
                    lastActiveWindow = activeWindow;
                    activeWindow = HELP_PANEL;
                    top_panel(m_panels[HELP_PANEL]);
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
                    top_panel(m_panels[activeWindow]);
                }
                break;
            default:
                break;
        }
    }
}

PANEL* Editor::createDefaultPanel() {
    const int y = getmaxy(stdscr);
    const int x = getmaxx(stdscr);
    WINDOW* const win = newwin(y, x, 0, 0);
    wattron(win, A_REVERSE);
    move(0, 0);
    wprintw (win, "Editor (press \"h\" for help)");
    int cursorY, cursorX;
    getyx(win, cursorY, cursorX);
    while (cursorY == 0) {
        waddch(win, ' ');
        getyx(win, cursorY, cursorX);
    }
    wattroff(win, A_REVERSE);
    return new_panel(win);
}

