// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <vector>
#include <string>
#include <menu.h>

#include "window.h"

class HelpWindow : public Window {
    public:
        HelpWindow();
        ~HelpWindow();

        void update() override;
        void handleKey(int key) override;

    private:
        MENU* m_menu;

        static std::vector<std::pair<std::string, std::string>> s_helpList;
        static ITEM** const s_items;
};

