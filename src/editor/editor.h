// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include "viewer.h"

class Editor : public Viewer {
    public:
        static std::optional<Glyph> editGlyph(Glyph glyph);

    private:
        static char** editorCompletion(const char* text, int start, int end);
        static char* editorCompletionGenerator(const char* text, int state);

        static const std::vector<std::pair<std::string, std::string>> editorCommands;
};

