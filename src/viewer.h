// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include "glyph.h"
#include "ui.h"

class Viewer : public UI {
    public:
        static void showGlyph(const Glyph& glyph);
        static void showGlyphs(const std::vector<Glyph>& glyphs);

    protected:
        static void printCoordinate(size_t coordinate);
        static bool printGlyphLine(const Glyph& glyph, size_t lineNumber);
};

