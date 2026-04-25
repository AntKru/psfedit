// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <array>
#include <memory>
#include <optional>
#include <ncurses.h>
#include <panel.h>

#include "theme.h"
#include "window.h"
#include "glyph.h"

class Editor {
    public:
        Editor();
        Editor(const Theme& theme);
        ~Editor();
        std::optional<Glyph> editGlyph(Glyph glyph);

    private:
        enum ActiveWindow {
            HELP_PANEL,
            OVERVIEW_PANEL,
            DEFAULT_PANEL,
            SAVE_PANEL,
            ActiveWindowSIZE,
        };

        std::array<std::unique_ptr<Window>, ActiveWindowSIZE> m_windows;

        void setActiveWindow(const Window& window);
};

