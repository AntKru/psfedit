// SPDX-License-Identifier: GPL-3.0-only

#include "themeWindow.h"
#include "theme.h"

ThemeWindow::ThemeWindow() {
    update();
    m_menu = new_menu(s_items);
    int winy, winx;
    getmaxyx(m_win, winy, winx);
    pushWindow(derwin(m_win, winy - 1, winx - 1, 1, 1));
    set_menu_win(m_menu, m_win);
    set_menu_sub(m_menu, getWindows().back());
    set_menu_fore(m_menu, COLOR_PAIR(Theme::C_PRIMARY));
    set_menu_back(m_menu, COLOR_PAIR(Theme::C_TERTIARY));
    set_menu_grey(m_menu, COLOR_PAIR(Theme::C_SECONDARY));
    post_menu(m_menu);
}

ThemeWindow::~ThemeWindow() {
    unpost_menu(m_menu);
    free_menu(m_menu);
}

void ThemeWindow::update() {
    Window::update();
    wresize(m_win, std::min(m_y - 4, 18), m_x - 4);
    move_panel(m_panel, 2, 2);
    box(m_win, 0, 0);
    mvwprintw(m_win, 0, 1, "Help");
}

void ThemeWindow::handleKey(int key) {
    switch (key) {
        case 'j':
        case KEY_DOWN:
            menu_driver(m_menu, REQ_DOWN_ITEM);
            break;
        case 'k':
        case KEY_UP:
            menu_driver(m_menu, REQ_UP_ITEM);
            break;

        case 'g':
        case 'r':
            menu_driver(m_menu, REQ_FIRST_ITEM);
            break;

        case 'G':
            menu_driver(m_menu, REQ_LAST_ITEM);
            break;

        case '\n':
            if (current_item(m_menu)) {
                ITEM* item = current_item(m_menu);
                Theme::setTheme(s_themeList.at(item_index(item)));
            }
            break;

        case KEY_MOUSE:
            handleMouse();
            break;
    }
}

std::vector<std::string> ThemeWindow::s_themeList = []() -> std::vector<std::string> {
    std::vector<std::string> themeList;
    themeList.reserve(Theme::colorschemes.size());
    for (const auto& theme : Theme::colorschemes) {
        themeList.push_back(theme.first);
    }
    return themeList;
} ();

ITEM** const ThemeWindow::s_items = []() -> ITEM** {
    ITEM** items = nullptr;
    items = new ITEM*[s_themeList.size() + 1];
    items[s_themeList.size()] = nullptr;
    for (size_t i = 0; i < s_themeList.size(); i++) {
        items[i] = new_item(s_themeList.at(i).c_str(), "");
    }
    return items;
} ();

void ThemeWindow::handleMouse() {
    MEVENT event;
    if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON4_PRESSED) {
            menu_driver(m_menu, REQ_SCR_ULINE);
        }
        if (event.bstate & BUTTON5_PRESSED) {
            menu_driver(m_menu, REQ_SCR_DLINE);
        }
    }
}

