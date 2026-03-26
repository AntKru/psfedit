// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include "viewer.h"

class Editor : public Viewer {
    public:
        static Glyph editGlyph(Glyph glyph);

    private:
        static void fillGlyph(Glyph& glyph, size_t x1, size_t y1, size_t x2, size_t y2);
        static void clearGlyph(Glyph& glyph);
        static void drawLine(Glyph& glyph, size_t x1, size_t y1, size_t x2, size_t y2);
        static void plotLine(Glyph& glyph, size_t x1, size_t y1, size_t x2, size_t y2, bool high);
        static char** editorCompletion(const char* text, int start, int end);
        static char* editorCompletionGenerator(const char* text, int state);

        static const std::vector<std::pair<std::string, std::string>> editorCommands;
};

