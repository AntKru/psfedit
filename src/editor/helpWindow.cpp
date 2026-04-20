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
    wresize(m_win, std::min(m_y - 4, 18), m_x - 4);
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

        case 'g':
        case 'r':
            menu_driver(m_menu, REQ_FIRST_ITEM);
            break;

        case 'G':
            menu_driver(m_menu, REQ_LAST_ITEM);

        case KEY_MOUSE:
            handleMouse();
            break;
    }
}

std::vector<std::pair<std::string, std::string>> HelpWindow::s_helpList = {
    {"Navigation:", ""},
    {"j, arrow key down", "Select next item / down"},
    {"k, arrow key up", "Select previous item / up"},
    {"h, arrow key left", "left"},
    {"l, arrow key right", "right"},
    {"g", "Go to top"},
    {"G", "Go to bottom"},
    {"General:", ""},
    {"?", "Show this help"},
    {"q", "Quit current view. If the current view is the default view, quit editor"},
    {"o", "Open overview"},
    {"r", "Reset view"},
    {"Editor - Navigation:", ""},
    {"Nav. keys + shift", "Fast movement"},
    {"Nav. keys + ctrl", "Very fast movement"},
    {"b", "Toggle pixel borders"},
    {"scroll wheel button", "Move cursor to pointer location"},
    {"Editor - editing:", ""},
    {"space", "Place marker"},
    {"left mouse button", "Place first marker"},
    {"right mouse button", "Place second marker"},
    {"u", "Undo"},
    {"U", "Redo"},
    {"t", "Toggle selected pixel"},
    {"e", "Toggle eraser. The eraser inverts the following commands"},
    {"1", "Draw a line between the markers"},
    {"2", "Draw a rectangle between the markers"},
    {"3", "Draw a circle. Marker 1 is the origin and it's distance to marker 2 the radius"},
    {"0", "Clear the drawing area. If eraser is on, fill it instead"},
    {"Overview:", ""},
    {"+", "Zoom in"},
    {"-", "Zoom out. Will be shown with incorrect aspect ratio"},
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

void HelpWindow::handleMouse() {
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

