// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <optional>
#include <ncurses.h>
#include <panel.h>

#include "glyph.h"

class Editor {
    public:
        Editor();
        ~Editor();
        std::optional<Glyph> editGlyph(Glyph glyph);

    private:
        static PANEL* createDefaultPanel();

        enum ActiveWindow {
            EDITOR_PANEL,
            OVERVIEW_PANEL,
            HELP_PANEL,
            DEFAULT_PANEL,
            ActiveWindowSIZE,
        };

        PANEL* m_panels[ActiveWindow::ActiveWindowSIZE] = {};
};

