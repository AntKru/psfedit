// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <vector>
#include <string>
#include <menu.h>

#include "window.h"

class ThemeWindow : public Window {
    public:
        ThemeWindow();
        ~ThemeWindow();

        void update() override;
        void handleKey(int key) override;

    private:
        MENU* m_menu;

        static std::vector<std::string> s_themeList;
        static ITEM** const s_items;

        void handleMouse();
};

