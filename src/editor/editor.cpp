// SPDX-License-Identifier: GPL-3.0-only

#include <string>

#include "editor.h"
#include "defaultWindow.h"
#include "overviewWindow.h"
#include "helpWindow.h"
#include "saveWindow.h"

Editor::Editor() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    start_color();
    mmask_t oldmask;
    mousemask(BUTTON1_CLICKED | BUTTON2_CLICKED | BUTTON3_CLICKED | BUTTON4_PRESSED | BUTTON5_PRESSED, &oldmask);
    mouseinterval(0);
    std::string message = "Your window might be too small";
    WINDOW* messageWin = newwin(3, message.size() + 2, 0, 0);
    box(messageWin, 0, 0);
    mvwprintw(messageWin, 1, 1, "%s", message.c_str());
    wrefresh(messageWin);
    m_windows.at(DEFAULT_PANEL) = std::make_unique<DefaultWindow>();
    m_windows.at(HELP_PANEL) = std::make_unique<HelpWindow>();
    m_windows.at(OVERVIEW_PANEL) = std::make_unique<OverviewWindow>();
    m_windows.at(SAVE_PANEL) = std::make_unique<SaveWindow>();
}

Editor::~Editor() {
    endwin();
}

std::optional<Glyph> Editor::editGlyph(Glyph glyph) {
    DefaultWindow& defaultWindow = dynamic_cast<DefaultWindow&>(*m_windows.at(DEFAULT_PANEL));
    OverviewWindow& overviewWindow = dynamic_cast<OverviewWindow&>(*m_windows.at(OVERVIEW_PANEL));
    SaveWindow& saveWindow = dynamic_cast<SaveWindow&>(*m_windows.at(SAVE_PANEL));
    ActiveWindow activeWindow = DEFAULT_PANEL;
    setActiveWindow(*m_windows.at(DEFAULT_PANEL));
    defaultWindow.setGlyph(glyph);

    while (saveWindow.getAction() == SaveWindow::Action::PENDING) {
        if (auto result = defaultWindow.getGlyph()) {
            glyph = *result;
        }
        overviewWindow.setGlyph(glyph);

        clear();
        for (const auto& windowp : m_windows) {
            if (windowp) {
                windowp->update();
            }
        }
        update_panels();
        doupdate();
        int ch = wgetch(m_windows.at(activeWindow)->getWindows().front());
        switch (ch) {
            case '?':
                if (activeWindow != HELP_PANEL) {
                    activeWindow = HELP_PANEL;
                } else {
                    activeWindow = DEFAULT_PANEL;
                }
                setActiveWindow(*m_windows.at(activeWindow));
                break;
            case 'o':
                if (activeWindow != OVERVIEW_PANEL) {
                    activeWindow = OVERVIEW_PANEL;
                } else {
                    activeWindow = DEFAULT_PANEL;
                }
                setActiveWindow(*m_windows.at(activeWindow));
                break;
            case 'q':
                if (activeWindow == DEFAULT_PANEL) {
                    if (defaultWindow.changed()) {
                        activeWindow = SAVE_PANEL;
                    } else {
                        saveWindow.setAction(SaveWindow::Action::FORCE_QUIT);
                    }
                } else {
                    activeWindow = DEFAULT_PANEL;
                }
                setActiveWindow(*m_windows.at(activeWindow));
                break;
            default:
                m_windows.at(activeWindow)->handleKey(ch);
                break;
        }
    }

    if (saveWindow.getAction() == SaveWindow::Action::SAVE_QUIT) {
        return defaultWindow.getGlyph();
    }

    return {};
}

void Editor::setActiveWindow(const Window& window) {
    for (PANEL* panel : window.getPanels()) {
        top_panel(panel);
    }
}

