// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <ncurses.h>

#include "editor.h"

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
}

Editor::~Editor() {
    endwin();
}

std::optional<Glyph> Editor::editGlyph(Glyph glyph) {
    return {};
}

