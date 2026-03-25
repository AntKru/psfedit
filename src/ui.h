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
            COMMAND_SIZE,
        };

        static std::pair<Command, unsigned short int> getCommand();
        static void showHeader(Psf::PsfHeader header);

    private:
        static char** menuCompletion(const char* text, int start, int end);
        static char* menuCompletionGenerator(const char* text, int state);

        static const std::vector<std::pair<std::string, std::string>> menuCommands;
};

