// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <optional>
#include <vector>
#include <ncurses.h>

#include "glyph.h"

class Editor {
    public:
        Editor();
        ~Editor();
        std::optional<Glyph> editGlyph(Glyph glyph);

    private:
        std::vector<WINDOW*> windows;
};

