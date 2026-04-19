// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <vector>
#include <panel.h>

class Window {
    public:
        Window();
        virtual ~Window();
        virtual void update();
        virtual void handleKey(int key);

        std::vector<WINDOW*> getWindows() const;
        std::vector<PANEL*> getPanels() const;

        enum ColorPairs {
            C_SELECTED = 1,
            C_CURSOR = 2,
            C_UI = 3,
            C_IMPORTANT = 4,
        };

    private:
        std::vector<WINDOW*> m_wins;
        std::vector<PANEL*> m_panels;

    protected:
        void pushWindow(WINDOW* win);
        bool popWindow();
        int m_y, m_x;
        WINDOW* const m_win;
        PANEL* const m_panel;
};

