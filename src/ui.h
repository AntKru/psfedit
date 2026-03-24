// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "glyph.h"

class UI {
    public:
        enum class Command {
            SHOW,
            EDIT,
            SAVE,
            EXIT,
            COMMAND_SIZE,
        };

        static std::pair<Command, unsigned short int> getCommand();

        static void showGlyph(const Glyph& glyph);
        static Glyph editGlyph(const Glyph& glyph);
    private:
        static void printGlyphLine(const std::vector<bool>& line, bool highlight = false, bool shrink = false);
        static char** menuCompletion(const char* text, int start, int end);
        static char* menuCompletionGenerator(const char* text, int state);
        static char** editorCompletion(const char* text, int start, int end);
        static char* editorCompletionGenerator(const char* text, int state);

        static const std::vector<std::pair<std::string, std::string>> menuCommands;
};

