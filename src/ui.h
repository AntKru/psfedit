// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <string>
#include <utility>
#include <vector>

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

    private:
        static char** menuCompletion(const char* text, int start, int end);
        static char* menuCompletionGenerator(const char* text, int state);

        static const std::vector<std::pair<std::string, std::string>> menuCommands;
};

