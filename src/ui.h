// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "psf.h"

class UI {
    public:
        enum class Command {
            SHOW,
            EDIT,
            SAVE,
            EXIT,
            HEADER,
            ADD_GLYPH_UNICODE,
            ADD_GLYPH_NO_UNICODE,
            COMMAND_SIZE,
        };

        static std::pair<Command, std::string> getCommand();
        static void showHeader(const Psf::PsfHeader& header);

    private:
        static char** menuCompletion(const char* text, int start, int end);
        static char* menuCompletionGenerator(const char* text, int state);

        static const std::vector<std::pair<std::string, std::string>> menuCommands;
};

