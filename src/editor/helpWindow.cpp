// SPDX-License-Identifier: GPL-3.0-only

#include "helpWindow.h"

HelpWindow::HelpWindow() {
    update();
    m_menu = new_menu(s_items);
    int winy, winx;
    getmaxyx(m_win, winy, winx);
    pushWindow(derwin(m_win, winy - 1, winx - 1, 1, 1));
    set_menu_win(m_menu, m_win);
    set_menu_sub(m_menu, getWindows().back());
    post_menu(m_menu);
}

HelpWindow::~HelpWindow() {
    unpost_menu(m_menu);
    free_menu(m_menu);
}

void HelpWindow::update() {
    Window::update();
    wresize(m_win, m_y - 4, m_x - 4);
    move_panel(m_panel, 2, 2);
    box(m_win, 0, 0);
    mvwprintw(m_win, 0, 1, "Help");
}

void HelpWindow::handleKey(int key) {
    switch (key) {
        case 'j':
        case KEY_DOWN:
            menu_driver(m_menu, REQ_DOWN_ITEM);
            break;
        case 'k':
        case KEY_UP:
            menu_driver(m_menu, REQ_UP_ITEM);
            break;
    }
}

std::vector<std::pair<std::string, std::string>> HelpWindow::s_helpList = {
    {"Menus:", ""},
    {"j, arrow key down", "Select next item"},
    {"k, arrow key up", "Select previous item"},
    {"General:", ""},
    {"h", "Show this help"},
    {"q", "Quit current view. If the current view is the default view, quit editor"},
};

ITEM** const HelpWindow::s_items = []() -> ITEM** {
    ITEM** items = nullptr;
    items = new ITEM*[s_helpList.size() + 1];
    items[s_helpList.size()] = nullptr;
    for (size_t i = 0; i < s_helpList.size(); i++) {
        items[i] = new_item(s_helpList.at(i).first.c_str(), s_helpList.at(i).second.c_str());
        if (s_helpList.at(i).second.empty()) {
            item_opts_off(items[i], O_SELECTABLE);
        }
    }
    return items;
} ();

