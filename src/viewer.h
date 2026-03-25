// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include "glyph.h"
#include "ui.h"

class Viewer : public UI {
    public:
        static void showGlyph(const Glyph& glyph);
        static void showGlyphs(const std::vector<Glyph>& glyphs);

    protected:
        static void printGlyphLine(const std::vector<bool>& line, bool highlight = false, bool shrink = false);
};

