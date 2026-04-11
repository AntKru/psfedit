// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <panel.h>

class Window {
    public:
        Window();
        virtual ~Window();
        virtual void update();
        virtual void handleKey(int key);

        WINDOW* getWindow() const;
        PANEL* getPanel() const;

    protected:
        int m_y, m_x;
        WINDOW* const m_win;
        PANEL* const m_panel;
};

